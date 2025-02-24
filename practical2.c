#include "practical2.h"
#include <stdio.h>
#include <stdlib.h>

/* Util functions, remove after use */
void print_matrix(matrix *m) {
  int r, c;
  for (r = 0; r < m->nrows; ++r) {
    for (c = 0; c < m->ncols; ++c) {
      printf("[%d] ", get_value(m, r, c));
    }
    printf("\n");
  }
}

void print_arr(int *arr, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    printf("%d: %d%s", i, arr[i], i == len - 1 ? "\n" : ", ");
  }
}
/* Util functions, remove after use */

int main(int argc, char **argv) {
  int *read = file_to_1D_array("example.txt");

  matrix *mat1 = create_matrix("example.txt", 3, 5);
  matrix *mat2 = create_matrix("example.txt", 5, 3);

  matrix *sum = matrix_addition(mat1, mat1);
  matrix *mult = matrix_multiplication(mat1, mat2);

  char *mat1_rep;
  char *mat2_rep;

  print_arr(read, 15);

  /*
    printf("mat1:\n");
    print_matrix(mat1);
    printf("mat2:\n");
    print_matrix(mat2);
    printf("sum:\n");
    print_matrix(sum);
    printf("mult:\n");
    print_matrix(mult);
  */

  mat1_rep = represent_matrix(mat1);
  mat2_rep = represent_matrix(mat2);
  printf("%s\n", mat1_rep);
  printf("%s\n", mat2_rep);

  free_matrix(mat1);
  free_matrix(mat2);
  free_matrix(sum);
  free_matrix(mult);
  free(mat1_rep);
  free(mat2_rep);

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
  matrix *m = (matrix *)malloc(sizeof(matrix));
  m->matrix = create_two_d_array(nrows, ncols);
  m->nrows = nrows;
  m->ncols = ncols;
  return m;
}

int get_value(matrix *m, int rownum, int column) {
  return m->matrix[rownum][column];
}

void set_value(matrix *m, int rownum, int column, int val) {
  m->matrix[rownum][column] = val;
}

matrix *create_matrix(char *filename, int nrows, int ncols) {
  int *arr = file_to_1D_array(filename);
  matrix *mat = create_empty_matrix(nrows, ncols);
  int r, c;
  for (r = 0; r < nrows; ++r) {
    for (c = 0; c < ncols; ++c) {
      set_value(mat, r, c, arr[(r * ncols) + c]);
    }
  }
  return mat;
}

matrix *matrix_addition(matrix *a, matrix *b) {
  int nrows = a->nrows;
  int ncols = a->ncols;
  matrix *res = create_empty_matrix(nrows, ncols);
  int r, c, a_val, b_val;
  for (r = 0; r < nrows; ++r) {
    for (c = 0; c < ncols; ++c) {
      a_val = get_value(a, r, c);
      b_val = get_value(b, r, c);
      set_value(res, r, c, a_val + b_val);
    }
  }
  return res;
}

matrix *matrix_multiplication(matrix *a, matrix *b) {
  int nrows = a->nrows;
  int ncols = b->ncols;
  int nvals = a->ncols;
  matrix *res = create_empty_matrix(nrows, ncols);
  int r, c, i, a_val, b_val, val;
  for (r = 0; r < nrows; ++r) {
    for (c = 0; c < ncols; ++c) {
      val = 0;
      for (i = 0; i < nvals; ++i) {
        a_val = get_value(a, r, i);
        b_val = get_value(b, i, c);
        val = val + (a_val * b_val);
      }
      set_value(res, r, c, val);
    }
  }
  return res;
}

void free_two_d_array(int **array, int nrows, int ncols) {
  int i;
  for (i = 0; i < nrows; ++i) {
    free(array[i]);
  }
  free(array);
}

void free_matrix(matrix *m) {
  free_two_d_array(m->matrix, m->nrows, m->ncols);
  free(m);
}

int repr_size(int num) {
  int s = 1;
  if (num < 0) {
    s += 1;
    num = -num;
  }
  while (num > 9) {
    num /= 10;
    ++s;
  }
  return s;
}

char *inttostr(int num) {
  int sz = repr_size(num);
  int i = sz - 1;
  char *str = malloc(sz * sizeof(char) + 1);
  str[sz] = '\0';
  if (num < 0) {
    str[0] = '-';
    num = -num;
  }
  do {
    str[i] = (num % 10) + '0';
    num /= 10;
    --i;
  } while (num);
  return str;
}

char *represent_matrix(matrix *m) {
  int buf_size = 0;
  int r, c;
  int pos = 0;
  char *val_str;
  char *repr;
  for (r = 0; r < m->nrows; ++r) {
    /* 5 chars per row ("[ " at start and " ]" at end, plus '\n' if not last row
     * or '\0' for last row) */
    buf_size += 5;
    for (c = 0; c < m->ncols; ++c) {
      buf_size += repr_size(get_value(m, r, c));
      /* Additional 2 chars per value except last val in row (", ") */
      buf_size += c == m->ncols ? 0 : 2;
    }
  }
  repr = malloc(buf_size * sizeof(char));
  for (r = 0; r < m->nrows; ++r) {
    pos += snprintf(repr + pos, buf_size - pos, "[ ");
    for (c = 0; c < m->ncols; ++c) {
      val_str = inttostr(get_value(m, r, c));
      pos += snprintf(repr + pos, buf_size - pos, "%s%s", val_str,
                      c == m->ncols - 1 ? "" : ", ");
      free(val_str);
    }
    pos += snprintf(repr + pos, buf_size - pos, " ]%c",
                    r == m->nrows - 1 ? '\0' : '\n');
  }
  return repr;
}
