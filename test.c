#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double rand_double(double lower, double upper) {
  double s = ((double)rand() / (double)RAND_MAX);
  double num = lower + s*(upper-lower);
  return num;
}

int main(void) {
  srand(time(NULL));

  for (int i = 0; i < 10; i++) {
    double n = rand_double(5, 10);
    printf("%lf\n", n);
  }

  return 0;
}

// n between 0 u-l
// add l
// n between l