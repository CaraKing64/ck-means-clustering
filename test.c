#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){

  srand((int)time(NULL));

  int upper = 5;
  int lower = 0;

  for (int i = 0; i < 10; i++){
    int x = (rand() % (upper - lower + 1)) + lower; 
    printf("%d\n", x);
  }


  return 0;
}