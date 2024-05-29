#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int ReadFileToDoubleArray(char* fname, double** end_array, int* p_rows,
                          int* p_cols) {
  int rows = *p_rows;
  int cols = *p_cols;

  // WENDYN via https://stackoverflow.com/questions/12911299/read-csv-file-in-c
  char str[128];
  int result;
  FILE* f = fopen(fname, "r");
  int count = 0;
  int comma_count = 0;

  do {
    result = fscanf(f, "%127[^;\n]", str);
    if (result == 0) {
      result = fscanf(f, "%*c");
    } else if (result == 1) {
      if (cols == -1) {
        comma_count = 0;
        for (int i = 0; i < strlen(str); i++) {
          if (str[i] == ',') {
            comma_count++;
          }
        }
      }

      count++;
    }
  } while (result != EOF);

  fclose(f);

  rows = count;
  cols = comma_count + 1;
  printf("n_values = %d  n_dimensions = %d\n", rows, cols);

  end_array = malloc(sizeof(double*) * rows);
  if (end_array == NULL) {
    printf("Memory allocation error.\n");
    return 1;
  }
  for (int r = 0; r < rows; r++) {
    end_array[r] = malloc(sizeof(double) * cols);
  }

  FILE* f2 = fopen(fname, "r");
  result = 0;
  int row = 0;
  do {
    result = fscanf(f2, "%127[^;\n]", str);
    if (result == 0) {
      result = fscanf(f2, "%*c");
    } else if (result == 1) {
      // printf("%d %s\n", result, str);

      char* token;
      int i = 0;
      token = strtok(str, ",");
      while (token != NULL) {
        end_array[row][i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
      }
      row++;
    }
  } while (result != EOF);
  return 0;
}

double Distance(double* p1, double* p2, int n_vars){
  double sum;
  for (int i = 0; i < n_vars; i++){
    double dist = p1[i] - p2[i];
    sum += dist*dist;
  }
  

}

// double** GenerateClusters
// void def AssignToClusters(double* data, double* centroids, int n_centroids)
// void def MoveClusters()

int main(int argc, char** argv) {
  char* data_filename;
  int n_values = -1;
  int n_dimensions = -1;
  int max_k;
  int attempts;

  // printf("You entered %d arguments\n", argc);

  if (argc < 2) {
    printf("You are missing arguments\n");
    return 1;
  }

  data_filename = argv[1];

  double** data_array;

  if (argc > 2) {
    printf("Your extra unused arguments are: ");
    for (int i = 2; i < argc; i++) {
      printf("'%s'", argv[i]);
      if (i != argc - 1) {
        printf(", ");
      }
    }
    printf("\n");
  }

  int read_value = ReadFileToDoubleArray(data_filename, data_array, &n_values, &n_dimensions);
  if (read_value == 1){
    printf("Read file error.\n");
    return 1;
  }

  double* origin = {0, 0, 0};

  // print all values in the dataset
  for (int r = 0; r < n_values; r++) {
    for (int c = 0; c < n_dimensions; c++) {
      printf("%.0lf %.0lf", data_array[r][c], Distance(data_array[1], origin, n_dimensions));
    }
    printf("\n");
  }

  for (int k = 1; k <= max_k; k++) {
    for (int attempt = 1; attempt < attempts; attempt++) {
    }
  }

  return 0;
}