/*
	Created by Ricardo Zilleruelo on 2006.
*/

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include "input.h"
#include "random.h"

INPUT_GROUP INPUT_GROUP_getInput(char *path){
    int c,state,i,j,k;
    
    INPUT_GROUP input_group;
    FILE *f;
    
    input_group = (INPUT_GROUP) malloc(sizeof(INPUT_GROUP_TYPE));
	input_group->D = 0;
	input_group->sequences_no = 0;
	input_group->categories_no = 0;
	input_group->label = NULL;
    
    f = fopen(path,"r");
    c = 0;
    state = 0;
    while(c != EOF && (c != '\n' || !state)){
        c = fgetc(f);
        switch(state){
            case 0:
                if(c == ' ' || c == '\t')
                    state = 2;
                else if(c != '\n' && c != EOF)
                    state = 1;
                break;
            case 1:
                if(c == '\n' || c == EOF)
                    input_group->D++;
                else if(c == ' ' || c == '\t'){
                    input_group->D++;
                    state = 2;
                }
                break;
            default:
                if(c != ' ' && c != '\t' && c != '\n' && c != EOF)
                    state = 1;
        }
    }
    fclose(f);

	f = fopen(path,"r");
    c = 0;
    state = 0;
    while(c != EOF){
        c = fgetc(f);
        switch(state){
            case 0:
                if(c != ' ' && c != '\t' && c != '\n')
                    state = 1;
                break;
            case 1:
                if(c == '\n'){
                    state = 2;
                }
                else if(c == EOF){
                    input_group->sequences_no++;
                }
                break;
            default:
                while(c == ' ' || c == '\t')
                    c = fgetc(f);
                if(c == '\n' || c == EOF){
                    state = 0;
                    input_group->sequences_no++;
                }
                else
                    state = 1;
        }
    }
    fclose(f);

	input_group->sequence = (SEQUENCE *) malloc(sizeof(SEQUENCE)*input_group->sequences_no);
	for(i = 0; i < input_group->sequences_no; i++){
		input_group->sequence[i] = (SEQUENCE) malloc(sizeof(SEQUENCE_TYPE));
		input_group->sequence[i]->inputs_no = 0;
	}
	i = 0;
    f = fopen(path,"r");
    c = 0;
    state = 0;
    while(c != EOF){
        c = fgetc(f);
        switch(state){
            case 0:
                if(c != ' ' && c != '\t' && c != '\n')
                    state = 1;
                break;
            case 1:
                if(c == '\n'){
                    state = 2;
                    input_group->sequence[i]->inputs_no++;
                }
                break;
            default:
                while(c == ' ' || c == '\t')
                    c = fgetc(f);
                if(c == '\n' || c == EOF){
                    state = 0;
                    i++;
                }
                else
                    state = 1;
        }
    }
    fclose(f);

	for(i = 0; i < input_group->sequences_no; i++){
		input_group->sequence[i]->input = (INPUT *) malloc(sizeof(INPUT)*input_group->sequence[i]->inputs_no);
	}
    f = fopen(path,"r");
    c = 0;
    state = 0;
    i = 0;
	j = 0;
    while(i < input_group->sequences_no){
        c = fgetc(f);
        switch(state){
            case 0:
                if(c != ' ' && c != '\t' && c != '\n')
                    state = 1;
                break;
            case 1:
                fseek(f,-2,SEEK_CUR);
				if(j >= input_group->sequence[i]->inputs_no){
					i++;
					j = 0;
					if(i >= input_group->sequences_no)
						break;
				}
				input_group->sequence[i]->input[j] = (INPUT) malloc(sizeof(INPUT_TYPE));
                input_group->sequence[i]->input[j]->X = (double *) malloc(sizeof(double)*input_group->D);
                for(k = 0; k < input_group->D; k++)
                    c = fscanf(f,"%lf",input_group->sequence[i]->input[j]->X+k);
                while(c != EOF && c != '\n')
                    c = fgetc(f);
                state = 2;
				j++;
                break;
            default:
                while(c == ' ' || c == '\t')
                    c = fgetc(f);
                if(c == '\n' || c == EOF){
                    state = 0;
                }
                else
                    state = 1;
        }
    }
    fclose(f);

    return input_group;
}


INPUT_GROUP INPUT_GROUP_getInputLabel(char *path, char*pathLabel, int categories_no){
  	int i,j;
    FILE *f;
    INPUT_GROUP input_group;
    
    input_group = INPUT_GROUP_getInput(path);
    input_group->categories_no = categories_no;
    f = fopen(pathLabel,"r");
    for(i = 0; i < input_group->sequences_no; i++)
        for(j = 0; j < input_group->sequence[i]->inputs_no; j++)
            fscanf(f,"%d",&(input_group->sequence[i]->input[j]->label));
    fclose(f);
    return input_group;
}

void INPUT_GROUP_norm(INPUT_GROUP input_group){
	int i,j,k;
	double min, max;
	for(k = 0; k < input_group->D; k++){
		min = DBL_MAX;
		max = -DBL_MAX;
		for(i = 0; i < input_group->sequences_no; i++){
			for(j = 0; j < input_group->sequence[i]->inputs_no; j++){
				if(min > input_group->sequence[i]->input[j]->X[k]){
					min = input_group->sequence[i]->input[j]->X[k];
				}
				if(max < input_group->sequence[i]->input[j]->X[k]){
					max = input_group->sequence[i]->input[j]->X[k];
				}
			}
		}
		
		for(i = 0; i < input_group->sequences_no; i++){
			for(j = 0; j < input_group->sequence[i]->inputs_no; j++){
				input_group->sequence[i]->input[j]->X[k] = 2 / (max-min) * (input_group->sequence[i]->input[j]->X[k] - min) - 1;
			}
		}
	}
}

void free_INPUT(INPUT input){
    free(input->X);
    free(input);
}

void free_SEQUENCE(SEQUENCE sequence){
	int i;

	for(i = 0; i < sequence->inputs_no; i++)
		free_INPUT(sequence->input[i]);
	free(sequence->input);
	free(sequence);
}
void INPUT_GROUP_free(INPUT_GROUP input_group){
    int i;
    
    for(i = 0; i < input_group->sequences_no; i++)
    	free_SEQUENCE(input_group->sequence[i]);
    free(input_group->sequence);
    free(input_group);
}

void INPUT_GROUP_swaprandom(INPUT_GROUP input_group){
	int i,ri,length;
	SEQUENCE aux,*x;
	
	x = input_group->sequence;
	length = input_group->sequences_no-1;
	randomize();
	for(i = 0; i <= length; i++){
		ri = (int)((double)rand())/RAND_MAX*length;
		aux = x[i];
		x[i] = x[ri];
		x[ri] = aux;
	}
	
}
