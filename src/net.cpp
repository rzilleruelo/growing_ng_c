#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "random.h"
#include "list.hpp"
#include "linkedlist.hpp"
#include "net.hpp"

static char *concatenar(const char *s1, const char *s2, const char *s3){
    unsigned int i,j;
    char *s;
    
    s = (char *) malloc(sizeof(char)*(strlen(s1)+strlen(s2)+strlen(s3)+1));
    for(i = 0; i < strlen(s1); i++)
        s[i] = s1[i];
    for(j = 0; j < strlen(s2); j++,i++)
        s[i] = s2[j];
    for(j = 0; j < strlen(s3); j++,i++)
        s[i] = s3[j];
    s[i] = 0;
    return s;
}

static double cmpNeuron(Neuron *n1, Neuron *n2){
	return n1->id-n2->id;
}
		
Link::Link(Neuron *r, Neuron *s, int age){
	this->r = r;
	this->s = s;
	this->age = age;
}

Link::~Link(){
}


Neuron::Neuron(int id, int D){
	this->id = id;
	this->w = new double[D];
	this->dw = new double[D];
	this->L = new LinkedList<Link *>();
}

Neuron::~Neuron(){
	Link *l;
	delete this->w;
	delete this->dw;
	this->L->reset();
	while(this->L->hasNext()){
		l = this->L->next();
		l->r->L->remove(l);
		l->s->L->remove(l);
		delete l;
	}
	delete this->L;
}



Net::Net(int m, int D, const char *id){
    int i,j;
	Neuron *n;
	
    this->D = D;
    this->m = m;
    this->id = strdup(id);
	this->N = new LinkedList<Neuron *>();
	randomize();
	for(i = 0; i < m; i++){
		n = new Neuron(i,D);
        for(j = 0; j < D; j++){
            n->w[j] = random_11();
		}
		this->N->add(n);
    }
}

Net::Net(){
}

Net::~Net(){
	free(id);
	this->N->reset();
	while(this->N->hasNext())
		delete this->N->next();
	delete this->N;
}

void Net::save(const char *path, const char *id){
	int i;
    char *s;
    FILE *f;
	Neuron *n;
	Link *l;
    
    this->N->sort(cmpNeuron);
    f = fopen(s = concatenar(path,id,"NET.txt"),"w");
    free(s);
    fprintf(f,"D:%d m:%d\n",this->D,this->m);
    fclose(f);
    
    f = fopen(s = concatenar(path,id,"NEURONS.txt"),"w");
    free(s);
	this->N->reset();
    while(this->N->hasNext()){
		n = this->N->next();
		if(n->L->isEmpty())
			fprintf(f,"id:%d nlinks:%d\n",n->id,0);
		else{
			fprintf(f,"id:%d nlinks:%d",n->id,n->L->size());
			n->L->reset();
			while(n->L->isBeforeLast()){
				l = n->L->next();
				fprintf(f," e1:%d e2:%d",l->r->id,l->s->id);
			}
			l = n->L->next();
			fprintf(f," e1:%d e2:%d\n",l->r->id,l->s->id);
		}
	}
    fclose(f);
    
    f = fopen(s = concatenar(path,id,"W.txt"),"w");
    free(s);
	this->N->reset();
    while(this->N->hasNext()){
		n = this->N->next();
		for(i = 0; i < this->D-1; i++)
			fprintf(f,"%f ",n->w[i]);
		fprintf(f,"%f\n",n->w[i]);
    }
    fclose(f);
}

Net* Net::load(const char *path, const char *id){
	Net* net;
	int i,j,nlinks,ids,idr;
    char *s;
    FILE *f;
	Neuron **N;
	Link *l;
	bool *links;
    
	net = new Net();
	net->id = strdup(id);
    f = fopen(s = concatenar(path,id,"NET.txt"),"r");
    free(s);
    fscanf(f,"D:%d m:%d\n",&(net->D),&(net->m));
    fclose(f);
	
	net->N = new LinkedList<Neuron *>();
	N = new Neuron*[net->m];
	f = fopen(s = concatenar(path,id,"W.txt"),"r");
    free(s);
	for(i = 0; i < net->m; i++){
		N[i] = new Neuron(i,net->D);
		for(j = 0; j < net->D-1; j++)
			fscanf(f,"%lf ",N[i]->w+j);
		fscanf(f,"%lf\n",N[i]->w+j);
		net->N->add(N[i]);
	}
    fclose(f);
	
    f = fopen(s = concatenar(path,id,"NEURONS.txt"),"r");
    free(s);
	links = new bool[net->m*(net->m+1)/2];
	for(i = 0; i < net->m*(net->m+1)/2; i++)
		links[i] = false;
	for(i = 0; i < net->m; i++){
		fscanf(f,"id:%d nlinks:%d ",&j,&nlinks);
		for(j = 0; j < nlinks; j++){
			fscanf(f,"e1:%d e2:%d ",&ids,&idr);
			if(ids < idr){
				int aux = ids;
				ids = idr;
				idr = aux;
			}
			if(!links[ids*(ids+1)/2+idr]){
				l = new Link(N[ids],N[idr],0);
				N[ids]->L->add(l);
				N[idr]->L->add(l);
				links[ids*(ids+1)/2+idr] = true;
			}
		}
	}
    fclose(f);
	delete N;
	return net;
}
