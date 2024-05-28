#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// void ReadFileToArray()

// int Distance(double x1, double y1, double x2, double y2)

// double** GenerateClusters
// void def AssignToClusters(double* data, double* centroids, int n_centroids)
// void def MoveClusters()


int main(int argc, char** argv){

  char* data_filename = argv[1];
  printf("You entered %d arguments\n", argc);

  if (argc == 1){
    printf("GRAHHHH");
    printf("Enter a filename\n");
    return 1;
  }

  if (argc > 2){
    printf("Your extra arguments are '");
    for (int i = 2; i < argc; i++){
       printf("%s ", argv[i]);
    }
    printf("'\n");
  }
  
  printf("You have entered the file as '%s'\n", data_filename);


  





  return 0;
}