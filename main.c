#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  int max_k;
  int attempts;

  // printf("You entered %d arguments\n", argc);

  if (argc < 4) {
    printf("You are missing arguments\n");
    return 1;
  }

  if (argc >= 4) {
    data_filename = argv[1];
    n_values = atoi(argv[2]);
    n_dimensions = atoi(argv[3]);
  }

  if (argc >= 5) {
    max_k = atoi(argv[4]);
  } else {
    max_k = n_values;
  }
  if (argc >= 6) {
    attempts = atoi(argv[5]);
  } else {
    attempts = 10;
  }

  if (argc > 6) {
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

  printf(
      "Performing k-means clustering from 0 to %d attempting %d times for each "
      "value of k.\nData is found at '%s' "
      "containing %d values in %d dimensions.\n",
      max_k, attempts, data_filename, n_values, n_dimensions);

  double** data_array = malloc(sizeof(double*) * n_values);
  for (int r = 0; r < n_values; r++) {
    data_array[r] = malloc(sizeof(double) * n_dimensions);
  }

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
        const char s[2] = ",";
        char* str1;
        char* str2;
        char copy_of_string[40];
        strcpy(copy_of_string, str);

        str1 = strtok(str, s);
        str2 = strtok(NULL, s);

        char* ptr;
        // printf("%d %s   Str1: %s  Str2: %s\n", count, copy_of_string, str1,
        //       str2);
        data_array[count][0] = strtod(str1, &ptr);
        data_array[count][1] = strtod(str2, &ptr);

        count++;
      }
    }

  } while (result != EOF);

  // print all values in the dataset
  for (int r = 0; r < n_values; r++) {
    for (int c = 0; c < n_dimensions; c++) {
      printf("%d ", (int)data_array[r][c]);
    }
    printf("\n");
  }

  for (int k = 1; k <= max_k; k++) {
    for (int attempt = 1; attempt < attempts; attempt++) {

    }
  }

  return 0;
}