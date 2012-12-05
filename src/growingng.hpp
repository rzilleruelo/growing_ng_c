#include <stdio.h>
#include "input.h"
template <class T> class LinkedList;

void growingNG_train(INPUT_GROUP inputs, Net *net, double epsilonb, double epsilonn, int amax, double velMinError, double minError,  int mmax, int meqmax, int alpha);
void growingNG_classify(INPUT_GROUP inputs, Net* net, LinkedList<int>** asign, FILE *f);
void growingNG_inputQuantization(INPUT_GROUP inputs, Net* net,FILE *f);