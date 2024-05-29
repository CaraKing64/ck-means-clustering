#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void) {
  int n_dimensions = 3;
  char str[] = "12,52,36";
  double* row = malloc(sizeof(double) * n_dimensions);
  char* token;


  int i = 0;
  token = strtok(str, ",");
  while (token != NULL){
    row[i] = atoi(token);
    token = strtok(NULL, ",");
    i++;
  }

  for (int i = 0; i < n_dimensions; i++){
    printf("%lf\n", row[i]);
  }


  return 0;
}