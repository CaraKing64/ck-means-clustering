#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

double Distance(double* p1, double* p2, int n_vars) {
  double sum;
  for (int i = 0; i < n_vars; i++) {
    double dist = p1[i] - p2[i];
    sum += dist * dist;
  }
  return sqrt(sum);
}

int main(void) {
  
  
  double p1[2];
  p1[0] = 0;
  p1[1] = 0;
  double p2[2];
  p2[0] = 3;
  p2[1] = 4;

  printf("%lf \n", EuclidianDistance(p1, p2, 2));


  return 0;
}