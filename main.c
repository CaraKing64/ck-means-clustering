#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int ReadFileToDoubleArray(char* fname, double*** output_array, int* p_rows,
                          int* p_cols) {
  int rows = *p_rows;
  int cols = *p_cols;

  // WENDYN via https://stackoverflow.com/questions/12911299/read-csv-file-in-c
  char str[128];
  int result;
  FILE* f = fopen(fname, "r");
  int count = 0;
  int comma_count = 0;
  // read csv file and count number of rows and columns
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

  double** end_array = malloc(sizeof(double*) * rows);
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

      // split the string where there are commas
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

  *p_rows = rows;
  *p_cols = cols;
  *output_array = end_array;

  return 0;
}

double Distance(double* p1, double* p2, int n_vars) {
  double sum;
  for (int i = 0; i < n_vars; i++) {
    double dist = p1[i] - p2[i];
    sum += dist * dist;
  }
  return sqrt(sum);
}

double RandDouble(double lower, double upper) {
  double s = ((double)rand() / (double)RAND_MAX);
  double num = lower + s * (upper - lower);
  return num;
}

int GenerateClusters(int n_clusters, int n_coordinates, double** min_vals,
                          double** max_vals, double*** end_array) {
  double** c_array = malloc(sizeof(double*) * n_clusters);

  double* mins = *min_vals;
  double* maxs = *max_vals;

  for (int r = 0; r < n_clusters; r++) {
    c_array[r] = malloc(sizeof(double) * n_coordinates);
    for (int c = 0; c < n_coordinates; c++) {
      c_array[r][c] = 0;
    }
  }

  for (int cluster = 0; cluster < n_clusters; cluster++) {
    for (int coord = 0; coord < n_coordinates; coord++) {
      c_array[cluster][coord] = RandDouble(mins[coord], maxs[coord]);
    }
  }

  for (int cluster = 0; cluster < n_clusters; cluster++) {
    for (int coord = 0; coord < n_coordinates; coord++) {
      printf("%lf ", c_array[cluster][coord]);
    }
    printf("\n");
  }
  *end_array = c_array;
  return 0;
}

int main(int argc, char** argv) {
  char* data_filename;
  int n_values = -1;
  int n_dimensions = -1;
  int start_k = -1;
  int max_k = -1;
  int attempts = -1;
  int void_num;

  srand(time(NULL));

  if (argc < 2) {
    printf("You are missing arguments\n");
    return 1;
  }
  data_filename = argv[1];
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-sk") == 0) {
      printf("ENTERED -sk %s\n", argv[i + 1]);
      start_k = atoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-mk") == 0) {
      printf("ENTERED -mk %s\n", argv[i + 1]);
      max_k = atoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-a") == 0) {
      printf("ENTERED -a %s\n", argv[i + 1]);
      attempts = atoi(argv[i + 1]);
    }
  }

  double** data_array;
  int read_value = ReadFileToDoubleArray(data_filename, &data_array, &n_values,
                                         &n_dimensions);
  if (read_value == 1) {
    printf("Read file error.\n");
    return 1;
  }

  printf("n_values = %d  n_dimensions = %d\n", n_values, n_dimensions);

  // each value in the dataset
  for (int r = 0; r < n_values; r++) {
    // each dimension in the dataset
    for (int c = 0; c < n_dimensions; c++) {
      printf("%.0lf ", data_array[r][c]);
    }
    printf("\n");
  }

  // find min and max for each variable
  double* mins = malloc(sizeof(double) * n_dimensions);
  double* maxs = malloc(sizeof(double) * n_dimensions);
  for (int i = 0; i < n_dimensions; i++) {
    mins[i] = data_array[0][i];
    maxs[i] = data_array[0][i];
  }

  // each value in the dataset
  for (int r = 0; r < n_values; r++) {
    // each dimension in the dataset
    for (int c = 0; c < n_dimensions; c++) {
      double val = data_array[r][c];
      if (val < mins[c]) {
        mins[c] = val;
      }
      if (val > maxs[c]) {
        maxs[c] = val;
      }
    }
  }

  // show min and max values for each variable
  printf("\nMin values: ");
  for (int i = 0; i < n_dimensions; i++) {
    printf("%.1lf ", mins[i]);
  }
  printf("\nMax values: ");
  for (int i = 0; i < n_dimensions; i++) {
    printf("%.1lf ", maxs[i]);
  }
  printf("\n");

  for (int k = start_k; k <= max_k; k++) {
    // initialise variables
    int finished = 0;
    // generate clusters
    printf("\nGENERATING %d CLUSTERS\n", k);
    double** cluster_array;
    int gen_cluster_return = GenerateClusters(k, n_dimensions, &mins, &maxs, &cluster_array);

    // while not finished
    while (finished == 0) {
      // assign points to clusters
      // move clusters to middle of new points

      // copy cluster_array to old_cluster_array
      double** old_cluster_array = malloc(sizeof(double*) * k);
      if (old_cluster_array == NULL) {
        printf("Memory allocation error");
        return 1;
      }
      for (int r = 0; r < k; r++) {
        old_cluster_array[r] = malloc(sizeof(double) * n_dimensions);
        if (old_cluster_array[r] == NULL) {
          printf("Memory allocation error");
          return 1;
        }
      }
      for (int r = 0; r < k; r++) {
        for (int c = 0; c < n_dimensions; c++) {
          old_cluster_array[r][c] = cluster_array[r][c];
        }
      }
      // print old_cluster_array
      printf("Old Cluster Array\n");
      for (int r = 0; r < k; r++) {
        for (int c = 0; c < n_dimensions; c++) {
          printf("%lf", old_cluster_array[r][c]);
        }
        printf("\n");
      }




      finished = 1;
    }

    // calculate distortion
  }

  return 0;
}

// array k, each element points to array of attempts
// each element in attempts points to a 2d array of original data set with an
// additional column of assigned cluster

// void def AssignToClusters(double* data, double* centroids, int n_centroids)
// void def MoveClusters()