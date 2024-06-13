#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void inc(int *p_x){
  (*p_x)++;
}

int main(void) {
  srand(time(NULL));

  int num = 5;
  printf("1. %d\n", num);
  inc(&num);
  printf("2. %d\n", num);

  return 0;
}

// n between 0 u-l
// add l
// n between l