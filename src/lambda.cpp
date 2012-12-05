#include <math.h>
#include <stdlib.h>
#include "lambda.hpp"

Lambda::Lambda(double lambdai, double lambdaf){
	this->lambdai = lambdai;
    this->lambdaf = lambdaf;
    this->lambdaf_div_lambdai = lambdaf/lambdai;
}

Lambda::~Lambda(){
}
		
double Lambda::value(int k, int k_max){
    return lambdai*pow(lambdaf_div_lambdai,(double)k/k_max);
}
