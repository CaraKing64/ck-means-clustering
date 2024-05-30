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

  // initialise c_array
  for (int r = 0; r < n_clusters; r++) {
    c_array[r] = malloc(sizeof(double) * n_coordinates);
    for (int c = 0; c < n_coordinates; c++) {
      c_array[r][c] = 0;
    }
  }
  // set values in c_array as randomly generates doubles
  for (int cluster = 0; cluster < n_clusters; cluster++) {
    for (int coord = 0; coord < n_coordinates; coord++) {
      c_array[cluster][coord] = RandDouble(mins[coord], maxs[coord]);
    }
  }
  // print c_array
  for (int cluster = 0; cluster < n_clusters; cluster++) {
    for (int coord = 0; coord < n_coordinates; coord++) {
      printf("%lf ", c_array[cluster][coord]);
    }
    printf("\n");
  }
  *end_array = c_array;
  return 0;
}

int AssignPointsToCentroids(int n_clusters, int n_values, int n_vars,
                            double*** data_array, double*** cluster_array,
                            double*** data_cluster_array) {
  printf("before printing centroids\n");
  for (int c = 0; c < n_clusters; c++) {
    for (int r = 0; r < n_vars; r++) {
      printf("%lf ", *cluster_array[r][c]);
    }
    printf("\n");
  }

  for (int i = 0; i < n_values; i++) {
    printf("Value i=%d ", i);

    for (int c = 0; c < n_clusters; c++) {
      printf("Dist c%d = %lf  ", c + 1,
             Distance(*data_array[i], *cluster_array[c], n_vars));
    }

    printf("\n");
  }
  return 0;
}

int main(int argc, char** argv) {
  char* data_filename;
  int n_values = -1;
  int n_dimensions = -1;
  int start_k = 0;
  int max_k = n_values;
  int attempts = 10;
  int void_num;

  // set random seed
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
    if (strcmp(argv[i], "-k") == 0) {
      printf("ENTERED -k %s\n", argv[i + 1]);
      start_k = atoi(argv[i + 1]);
      max_k = atoi(argv[i + 1]);
    }

    if (strcmp(argv[i], "-a") == 0) {
      printf("ENTERED -a %s\n", argv[i + 1]);
      attempts = atoi(argv[i + 1]);
    }
  }

  // read csv file containing data to array
  double** data_array;
  int read_value = ReadFileToDoubleArray(data_filename, &data_array, &n_values,
                                         &n_dimensions);
  if (read_value == 1) {
    printf("Read file error.\n");
    return 1;
  }

  printf("n_values = %d  n_dimensions = %d\n", n_values, n_dimensions);

  // print data
  for (int r = 0; r < n_values; r++) {
    for (int c = 0; c < n_dimensions; c++) {
      printf("%.0lf ", data_array[r][c]);
    }
    printf("\n");
  }

  // initialise mins and maxs
  double* mins = malloc(sizeof(double) * n_dimensions);
  double* maxs = malloc(sizeof(double) * n_dimensions);
  for (int i = 0; i < n_dimensions; i++) {
    mins[i] = data_array[0][i];
    maxs[i] = data_array[0][i];
  }
  // find min and max for each variable
  for (int r = 0; r < n_values; r++) {
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

  // print mins and maxs
  printf("\nMin values: ");
  for (int i = 0; i < n_dimensions; i++) {
    printf("%.1lf ", mins[i]);
  }
  printf("\nMax values: ");
  for (int i = 0; i < n_dimensions; i++) {
    printf("%.1lf ", maxs[i]);
  }
  printf("\n");

  // K-MEANS CLUSTERING LOOP

  // number of clusters
  for (int k = start_k; k <= max_k; k++) {
    // number of attempts for this value of k
    for (int attempt = 0; attempt < attempts; attempt++) {
      // initialise variables
      int finished = 0;
      // generate clusters
      printf("\nATTEMPT %d GENERATING %d CLUSTERS\n", attempt + 1, k);
      double** centroid_array;
      int gen_cluster_return =
          GenerateClusters(k, n_dimensions, &mins, &maxs, &centroid_array);

      // while not finished
      while (finished == 0) {
        // copy centroid_array to old_centroid_array
        double** old_centroid_array = malloc(sizeof(double*) * k);
        if (old_centroid_array == NULL) {
          printf("Memory allocation error");
          return 1;
        }
        for (int r = 0; r < k; r++) {
          old_centroid_array[r] = malloc(sizeof(double) * n_dimensions);
          if (old_centroid_array[r] == NULL) {
            printf("Memory allocation error");
            return 1;
          }
        }
        for (int r = 0; r < k; r++) {
          for (int c = 0; c < n_dimensions; c++) {
            old_centroid_array[r][c] = centroid_array[r][c];
          }
        }
        // print old_centroid_array
        printf("Old Cluster Array\n");
        for (int r = 0; r < k; r++) {
          for (int c = 0; c < n_dimensions; c++) {
            printf("%lf ", old_centroid_array[r][c]);
          }
          printf("\n");
        }

        // assign points to clusters

        // size k rows and up to (n_values+1) columns
        //  each row is a centroid and list all all values in dataset
        //  each value is represented with a number from 0 -> n_values
        //  representing its index in data_array
        double** cluster_data_array = malloc(sizeof(double*) * k);
        if (cluster_data_array == NULL) {
          printf("Memory allocation error\n");
          return 1;
        }
        for (int r = 0; r < k; r++) {
          cluster_data_array[r] = malloc(sizeof(double) * n_values);
          if (cluster_data_array[r] == NULL) {
            printf("Memory allocation error\n");
            return 1;
          }
        }

        printf("\nASSINGING POINTS TO %d CENTROIDS\n", k);
        int centroid_assign =
            AssignPointsToCentroids(k, n_values, n_dimensions, &data_array,
                                    &centroid_array, &cluster_data_array);
        if (centroid_assign != 0) {
          printf("Assigning points to clusters failed\n");
          return 1;
        }

        // move clusters to middle of new points

        finished = 1;
      }
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