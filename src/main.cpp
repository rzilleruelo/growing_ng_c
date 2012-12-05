#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "list.hpp"
#include "linkedlist.hpp"
#include "net.hpp"
#include "growingng.hpp"

static char *concatenar(char *s1, char *s2, char *s3){
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

void shortHelpTrain(){printf("Train: -t\n");}
void helpTrain(){
	shortHelpTrain();
	printf("Argumentos:\n\tchar *inputPath\n\tint m\n\tdouble epsilonb\n\tdouble epsilonn\n\tint amax\n\tdouble velMinError\n\tdouble minError\n\tint mmax\n\tint nmmax\n\tint alpha\n\tchar *outputPath\n\tchar *outputNetId\n");
}

void train(char *inputPath, int m, double epsilonb, double epsilonn, int amax, double velMinError, double minError, int mmax, int nmmax, int alpha, char *outputPath, char *outputNetId){
	INPUT_GROUP inputs;
	Net *net;
	
	printf("Loading input\n");
	inputs = INPUT_GROUP_getInput(inputPath);
	INPUT_GROUP_norm(inputs);
	
	printf("Crating network\n");
	net = new Net(m,inputs->D,outputNetId);
	printf("Training\n");
	growingNG_train(inputs,net,epsilonb,epsilonn,amax,velMinError,minError,mmax,nmmax,alpha);
	delete inputs;
	net->save(outputPath,outputNetId);
	delete net;
}


void shortHelpClassify(){printf("Classify: -c\n");}
void helpClassify(){
	shortHelpClassify();
	printf("\tchar* inputPath\n\tchar* netpath\n\tchar* netid\n\tchar* outputpath\n\tchar* outputid\n");
}
void classify(char* inputPath, char* netpath, char* netid, char* outputpath, char* outputid){
	int i;
	INPUT_GROUP input_group;
	Net *net;
	LinkedList<int> **asing;
	FILE *f;
	char *s;
	
	net = Net::load(netpath,netid);
	input_group = INPUT_GROUP_getInput(inputPath);
	INPUT_GROUP_norm(input_group);
	
	asing = new LinkedList<int>*[net->m];
	for(i = 0; i < net->m; i++)
		asing[i] = new LinkedList<int>();
	f = fopen(s = concatenar(outputpath,outputid,(char *) "Class.txt"),"w");
	free(s);
	growingNG_classify(input_group,net,asing,f);
	INPUT_GROUP_free(input_group);
	fclose(f);
	
	f = fopen(s = concatenar(outputpath,outputid, (char *) "Index.txt"),"w");
	free(s);
	for(i = 0; i < net->m; i++){
		asing[i]->reset();
		fprintf(f,"%d %d",i,asing[i]->size());
		while(asing[i]->hasNext()){
			fprintf(f," %d",asing[i]->next());
		}
		fprintf(f,"\n");
	}
	fclose(f);
	for(i = 0; i < net->m; i++)
		delete asing[i];
	delete asing;
	delete net;
}

void shortHelpQuantify(){printf("Quantify: -q\n");}
void helpQuantify(){
	shortHelpQuantify();
	printf("\tchar* inputPath\n\tchar* netpath\n\tchar* netid\n\tchar* outputpath\n");
}
void quantify(char* inputPath, char* netpath, char* netid, char* outputpath){
	INPUT_GROUP input_group;
	Net *net;
	FILE *f;
	
	net = Net::load(netpath,netid);
	input_group = INPUT_GROUP_getInput(inputPath);
	INPUT_GROUP_norm(input_group);
	
	f = fopen(outputpath,"w");
	
	growingNG_inputQuantization(input_group,net,f);
	fclose(f);
	INPUT_GROUP_free(input_group);
	delete net;
}

void printHelp(){
	shortHelpTrain();
	shortHelpClassify();
	shortHelpQuantify();
}

int main(int argc, char* argv[]){
	if(argc < 2){
		printHelp();
	}
	else if(!strcmp(argv[1],"-t")){
		if(argc != 14)
			helpTrain();
		else
			train(argv[2], atoi(argv[3]), atof(argv[4]), atof(argv[5]), atoi(argv[6]), atof(argv[7]), atof(argv[8]),atoi(argv[9]),atoi(argv[10]),atoi(argv[11]), argv[12], argv[13]);
	}
	else if(!strcmp(argv[1],"-c")){
		if(argc != 7)
			helpClassify();
		else
			classify(argv[2],argv[3],argv[4],argv[5],argv[6]);
	}
	else if(!strcmp(argv[1],"-q")){
		if(argc != 6)
			helpQuantify();
		else
			quantify(argv[2],argv[3],argv[4],argv[5]);
	}
	else{
		printHelp();
	}
}
