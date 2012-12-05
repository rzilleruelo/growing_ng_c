#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include "plot.h"
#include "net.hpp"
#include "list.hpp"

typedef struct NODO{
	int pid;
	struct NODO* sgte;
}NODO;

NODO *sonspid = NULL;


static void func(int x){
	NODO *p;
	
	p = sonspid;
	while(p != NULL){
		kill(p->pid,SIGKILL);
		p = p->sgte;
	}
	exit(0);
}

PLOT PLOT_new(){
	int fd[2];	
	char *args[2];
	PLOT p;
	NODO *nodo;

	p = (PLOT) malloc(sizeof(PLOT_TYPE));
	pipe(fd);
	
	if((p->pid = fork()) == 0){
		close(0);dup(fd[0]);close(fd[0]);
		args[0] = (char *) "ploter";
		args[1] = NULL;
		printf("%d\n",execv("./ploter",args));
	}
	
	nodo = (NODO *) malloc(sizeof(NODO));
	nodo->pid = p->pid;
	nodo->sgte = sonspid;
	sonspid = nodo;
	p->fd = fd[1];
	signal(SIGINT,func);
	return p;
}

void PLOT_3dcoos(PLOT plot, List<Neuron *> *neurons, int length){
	char c;
	Neuron *n;
	
	plot->cp = (unsigned char *)&length;
	c = 0;
	write(plot->fd,&c,sizeof(char));
	write(plot->fd,plot->cp,sizeof(int));
	while(neurons->hasNext()){
		n = neurons->next();
		plot->cp = (unsigned char *) n->w;
		write(plot->fd,plot->cp,sizeof(double)*3);
	}
}

void PLOT_3dcoos(PLOT plot, double **inputs, int length){
	char c;
	int i;
	
	plot->cp = (unsigned char *)&length;
	c = 0;
	write(plot->fd,&c,sizeof(char));
	write(plot->fd,plot->cp,sizeof(int));
	for(i = 0; i < length; i++){
		plot->cp = (unsigned char *) inputs[i];
		write(plot->fd,plot->cp,sizeof(double)*3);
	}
}

void PLOT_edges(PLOT plot, List<Neuron *> *neurons, int nedges){
	char c;
	Neuron *n;
	Link *l;
	
	plot->cp = (unsigned char *)&nedges;
	c = 2;
	write(plot->fd,&c,sizeof(char));
	write(plot->fd,plot->cp,sizeof(int));
	
	while(neurons->hasNext()){
		n = neurons->next();
		plot->edge[0] = n->id;
		n->L->reset();
		while(n->L->hasNext()){
			l = n->L->next();
			plot->edge[1] = l->r->id == n->id ? l->s->id : l->r->id;
			plot->cp = (unsigned char *) plot->edge;
			write(plot->fd,plot->cp,sizeof(int) * 2);
		}
	}
}


void PLOT_free(PLOT plot){
	NODO *p,*aux;
	
	kill(plot->pid,9);
	while(wait(&(plot->status)) != plot->pid);
	close(plot->fd);
	
	if((sonspid->pid = plot->pid)){
		p = sonspid;
		sonspid = sonspid->sgte;
		free(p);
	}
	else{
		p = sonspid;
		while(p->sgte->pid != plot->pid)
			p = p->sgte;
		aux = p->sgte;
		p->sgte = p->sgte->sgte;
		free(aux);
	}
	free(plot);
}
