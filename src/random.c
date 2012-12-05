#include <stdlib.h>
#include <time.h>
#include "random.h"

#pragma GCC diagnostic ignored "-Wuninitialized"
void randomize(){

  int srd, tmp;
               
  time_t t;
  struct tm *ts;

  time(&t);
  ts = localtime(&t);

  tmp = ts->tm_mday | (ts->tm_min << 6) | (ts->tm_sec << 12);

  if(srd == 0)
    srd |= tmp;
  else
    srd *= tmp;

  srand((unsigned)srd);
}

double random01(){
       return 1.0*rand()/RAND_MAX;
}

double random_11(){
       return 2.0*rand()/RAND_MAX-1.0;
}
