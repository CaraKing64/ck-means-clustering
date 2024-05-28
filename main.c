#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// void ReadFileToArray()

// int Distance(double x1, double y1, double x2, double y2)

// double** GenerateClusters
// void def AssignToClusters(double* data, double* centroids, int n_centroids)
// void def MoveClusters()

int main(int argc, char** argv) {
  char* data_filename;
  int n_values;
  int n_dimensions;

  // printf("You entered %d arguments\n", argc);

  if (argc < 4) {
    printf("You are missing arguments\n");
    return 1;
  }

  if (argc == 4) {
    data_filename = argv[1];
    n_values = atoi(argv[2]);
    n_dimensions = atoi(argv[3]);
  }

  if (argc > 4) {
    data_filename = argv[1];
    printf("Your extra unused arguments are: ");
    for (int i = 3; i < argc; i++) {
      printf("'%s'", argv[i]);
      if (i != argc - 1) {
        printf(", ");
      }
    }
    printf("\n");
  }

  printf("File '%s' with %d data values in %d dimensions\n", data_filename,
         n_values, n_dimensions);

  double* data_array = malloc(sizeof(double));




  // WENDYN via https://stackoverflow.com/questions/12911299/read-csv-file-in-c
  char str[128];
  int result;
  FILE* f = fopen(data_filename, "r");
  int count = 0;
  do {
    result = fscanf(f, "%127[^;\n]", str);

    if (result == 0) {
      result = fscanf(f, "%*c");
    } else {
      // Put here whatever you want to do with your value.
      if (count < n_values) {
        
        printf("%d    ", count);

        char str1[20];
        char str2[20];
        int string_number = 1;
        int string_index = 0;
        for (int i = 0; i < 40; i++){
          char c = str[i];
          printf("%c", str[i]);
          if (c == ','){
            //printf("FOUND COMMA");
            string_number = 2;
          } else if (string_number == 1){
            str1[string_index] = c;
          } else {
            //printf("GA");
            str2[string_index] = c;
          }
          string_index++;

          
        }
        printf(" Str1: %s  Str2: %s\n", str1, str2);
        
        count++;
      }
    }

  } while (result != EOF);

  return 0;
}