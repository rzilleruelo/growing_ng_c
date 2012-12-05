#include <stdio.h>
#include <float.h>
#include <math.h>
#include "list.hpp"
#include "linkedlist.hpp"
#include "plot.h"
#include "net.hpp"
#include "growingng.hpp"

static int
	m,
	D,
	amax,
	nedges,
	mmax;
	
static double
	epsilonb,
	epsilonn,
	lastError,
	currentError,
	minError;
	
static List<Neuron *> *N;

static Neuron *s1, *s2;

static Net *net;

static void init_training(Net *_net, double _epsilonb, double _epsilonn, int _amax, double _minError, int _mmax){
	Neuron *n;
	int i;
	
	net = _net;
    m = net->m;
    D = net->D;
	N = net->N;
	nedges = 0;
	N->reset();
	i = 0;
	while(N->hasNext()){
		n = N->next();
		n->id = i++;
		nedges += n->L->size();
		n->e = 0;
		n->activation = 0;
	}
	
	epsilonb = _epsilonb;
	epsilonn = _epsilonn;
	amax = _amax;
	lastError = 0;
	currentError = 0;
	minError = _minError;
	mmax = _mmax;
}

static void end_training(){
}

static inline double distance(double* X1, double* X2, double *dX){
	int i;
	double d;
	
	d = 0;
	for(i = 0; i < D; i++)
		d += (dX[i] = X1[i]-X2[i])*dX[i];
	return d;
}

static inline void present_input(double *input){
	int i;
	bool noExist;
	Link *l;
	Neuron *n;
	double e1,e2,e;
	
	N->reset();
	s1 = N->next();
	
	e1 = distance(input,s1->w,s1->dw);
	s2 = N->next();
	e2 = distance(input,s2->w,s2->dw);
	if(e2 < e1){
		e = e1; e1 = e2; e2 = e;
		n = s1; s1 = s2; s2 = n;
	}
	
	while(N->hasNext()){
		n = N->next();
		e = distance(input,n->w,n->dw);
		if(e < e1){
			e2 = e1;
			s2 = s1;
			e1 = e;
			s1 = n;
		}
		else if(e < e2){
			e2 = e;
			s2 = n;
		}
	}
	
	s1->e += e1;
	s1->activation++;
	currentError += e1;
	
	noExist = true;
	s1->L->reset();
	while(s1->L->hasNext()){
		l = s1->L->next();
		if(l->r == s2 || l->s == s2){
			noExist = false;
			l->age = 0;
		}
		else{
			l->age++;
			if(l->age > amax){
				l->r->L->remove(l);
				l->s->L->remove(l);
				delete l;
				nedges--;
			}
		}
	}
	
	if(noExist){
		s1->L->add(l = new Link(s1,s2,0));
		s2->L->add(l);
		nedges++;
	}

	for(i = 0; i < D; i++)
		s1->w[i] += epsilonb*s1->dw[i];
		
	s1->L->reset();
	while(s1->L->hasNext()){
		l = s1->L->next();
		n = l->r == s1 ? l->s : l->r;
		for(i = 0; i < D; i++)
			n->w[i] += epsilonn*n->dw[i];
	}
}

static double cmp(Neuron *n1, Neuron *n2){
	return n2->e-n1->e;
}

static void removeLazyNeurons(){
	int i;
	Link *l;
	Neuron *n;
	
	N->reset();
	while(N->hasNext()){
		n = N->next();
		if(m > 2 && n->activation == 0){
			n->L->reset();
			while(n->L->hasNext()){
				l = n->L->next();
				(l->r == n ? l->s : l->r)->L->remove(l);
				nedges--;
			}
			N->remove(n);
			m--;
			net->m = m;
			delete n;
		}
	}
	i = 0;
	N->reset();
	while(N->hasNext())
		N->next()->id = i++;
}



static inline void addUnits(){
	List<Neuron *> *S;
	Link *l,*laux;
	Neuron *newn,*neaux,*nemax;
	int i;
	S = new LinkedList<Neuron *>();
	
	N->reset();
	while(N->hasNext()){
		newn = N->next();
		newn->e /= newn->activation;
	}
	N->sort(cmp);
	N->reset();
	while(N->hasNext() && m < mmax){
		s1 = N->next();
		if(!S->contains(s1) && s1->e > minError){
			S->add(s1);
			
			nemax = NULL;
			s1->L->reset();
			while(s1->L->hasNext()){ 
				laux = s1->L->next();
				neaux = laux->r == s1 ? laux->s : laux->r;
				if(!S->contains(neaux)){
					nemax = neaux;
					l = laux;
					break;
				}
			}
			
			if(nemax != NULL){
				S->add(nemax);
				while(s1->L->hasNext()){
					laux = s1->L->next();
					neaux = laux->r == s1 ? laux->s : laux->r;
					if(!S->contains(neaux)){
						S->add(neaux);
						if(neaux->e > nemax->e){
							nemax = neaux;
							l = laux;
						}
					}
				}
				
				newn = new Neuron(m++,D);
				N->add(newn);
				net->m = m;
				
				s1->L->remove(l);
				nemax->L->remove(l);
				delete l;
				
				s1->L->add(l = new Link(s1,newn,0));
				newn->L->add(l);
				
				newn->L->add(l = new Link(newn,nemax,0));
				nemax->L->add(l);
				nedges++;
				
				for(i = 0; i < D; i++)
					newn->w[i] = 0.5*(s1->w[i]+nemax->w[i]);
			}
		}
	}
	delete S;
	
	i = 0;
	N->reset();
	while(N->hasNext())
		N->next()->id = i++;
}

void growingNG_train(INPUT_GROUP inputs, Net *net, double epsilonb, double epsilonn, int amax, double velMinError, double minError, int mmax, int neqmax, int alpha){
	int i,t,k,mold,nmeq;
	PLOT plot,plot2;
	Neuron *n;
	SEQUENCE sequence;
	int inputs_no;
	
	init_training(net,epsilonb,epsilonn,amax,minError,mmax);
	plot = plot2 = NULL;
	if(D == 3){
		//plot = PLOT_new();
		//plot2 = PLOT_new();
		//PLOT_3dcoos(plot2,inputs->x,inputs->inputs_no);
	}
	inputs_no = 0;
	for(i = 0; i < inputs->sequences_no; i++){
		sequence = inputs->sequence[i];
		for(t = 0; t < sequence->inputs_no; t++)
			inputs_no++;
	}
	
	k = 1;
	mold = m;
	nmeq = 0;
	while(true){
		INPUT_GROUP_swaprandom(inputs);
		for(i = 0; i < inputs->sequences_no; i++){
			sequence = inputs->sequence[i];
			for(t = 0; t < sequence->inputs_no; t++)
				present_input(sequence->input[t]->X);
		}
		
		currentError /= inputs_no;
		printf("n inputs:%d m:%4d D:%d edges:%6d eb:%f en:%f amax:%d minError:%f velError:%+e error:%f alpha:%d\n",inputs_no,m,D,nedges,epsilonb,epsilonn,amax,minError,lastError-currentError,currentError,alpha);
		if(currentError <= minError || (m == mmax && nmeq == neqmax*alpha))
			break;
		if(k >= alpha && lastError-currentError < velMinError*currentError && currentError > minError){
			removeLazyNeurons();
			addUnits();
			k = 1;
		}
		lastError = currentError;
		currentError = 0;
		N->reset();
		while(N->hasNext()){
			n = N->next();
			n->e = 0;
			n->activation = 0;
		}
		k++;
		
		if(mold == m){
			nmeq++;
		}
		else{
			nmeq = 0;
		}
		mold = m;
		
		if(plot != NULL){
			N->reset();
			PLOT_3dcoos(plot,N,m);
			N->reset();
			PLOT_edges(plot,N,2*nedges);
		}
	}
	removeLazyNeurons();
	if(plot != NULL){
		N->reset();
		PLOT_3dcoos(plot,N,m);
		N->reset();
		PLOT_edges(plot,N,2*nedges);
	}
	end_training();
	printf("Complete\n");
	if(plot != NULL){
		getchar();
		PLOT_free(plot);
		PLOT_free(plot2);
	}
}

