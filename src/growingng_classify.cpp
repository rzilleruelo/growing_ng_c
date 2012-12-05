#include <stdio.h>
#include <float.h>
#include <math.h>
#include "list.hpp"
#include "linkedlist.hpp"
#include "plot.h"
#include "net.hpp"
#include "growingng.hpp"

int D;

static inline double distance(double* X1, double* X2, double *dX){
	int i;
	double d;
	
	d = 0;
	for(i = 0; i < D; i++)
		d += (dX[i] = X1[i]-X2[i])*dX[i];
	return d;
}

void growingNG_classify(INPUT_GROUP inputs, Net* net, LinkedList<int>** asign, FILE *f){
	int i,j,t,winner,total;
	D = net->D;
	double dmin,d;
	Neuron *n;
	
	total = 0;
	for(i = 0; i < inputs->sequences_no; i++){
		for(t = 0; t < inputs->sequence[i]->inputs_no; t++){
			net->N->reset();
			n = net->N->next();
			dmin = distance(inputs->sequence[i]->input[t]->X,n->w,n->dw);
			winner = 0;
			j = 1;
			while(net->N->hasNext()){
				n = net->N->next();
				d = distance(inputs->sequence[i]->input[t]->X,n->w,n->dw);
				if(d < dmin){
					winner = j;
					dmin = d;
				}
				j++;
			}
			total++;
			asign[winner]->add(total);
			fprintf(f,"%d %d\n",total,winner);
		}
	}
}

void growingNG_inputQuantization(INPUT_GROUP inputs, Net* net,FILE *f){
	int i,j,t;
	D = net->D;
	double dmin,d;
	Neuron *n,*winner;
	
	for(i = 0; i < inputs->sequences_no; i++){
		for(t = 0; t < inputs->sequence[i]->inputs_no; t++){
			net->N->reset();
			winner = net->N->next();
			dmin = distance(inputs->sequence[i]->input[t]->X,winner->w,winner->dw);
			j = 1;
			while(net->N->hasNext()){
				n = net->N->next();
				d = distance(inputs->sequence[i]->input[t]->X,n->w,n->dw);
				if(d < dmin){
					winner = n;
					dmin = d;
				}
				j++;
			}
			fprintf(f,"%d",winner->id);
			for(j = 0; j < D; j++)
				fprintf(f,"\t%f",winner->w[j]);
			fprintf(f,"\n");
		}
		fprintf(f,"\n");
	}
}
