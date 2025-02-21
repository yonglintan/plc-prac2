#include "practical2.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int i;
  int *read = file_to_1D_array("example.txt");
  for (i = 0; i < 15; i++) {
    printf("%d: %d \n", i, read[i]);
  }
  printf("\n");
  return 0;
}

/* DO NOT MODIFY THIS FUNCTION */
int *file_to_1D_array(const char *filename) {
  FILE *f = fopen(filename, "r");
  int n, *a, c = 0;
  if (!f) {
    perror("Error opening file");
    return NULL;
  }
  if (fscanf(f, "%d", &n) != 1 || n <= 0) {
    perror("Invalid size");
    fclose(f);
    return NULL;
  }
  if (!(a = malloc(n * sizeof(int)))) {
    perror("Memory allocation failed");
    fclose(f);
    return NULL;
  }
  while (c < n && fscanf(f, "%d", &a[c]) == 1)
    c++;
  fclose(f);
  return a;
}

/* empty line just in case! */

int **create_two_d_array(int nrows, int ncols) {
  int **m = malloc(sizeof(int *) * nrows);
  int i;
  for (i = 0; i < nrows; i++) {
    m[i] = (int *)malloc(sizeof(int) * ncols);
  }
  return m;
}

matrix *create_empty_matrix(int nrows, int ncols) {
  static matrix m;
  m.matrix = create_two_d_array(nrows, ncols);
  m.nrows = nrows;
  m.ncols = ncols;
  return &m;
}
