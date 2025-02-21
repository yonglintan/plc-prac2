/* Do NOT remove */
int *file_to_1D_array(const char *filename);

/* Part 1 */
int **create_two_d_array(int nrows, int ncols);
/* here is your custom matrix type: */
typedef struct {
  /* the char below is so the code can compile */
  int **matrix;
  int nrows;
  int ncols;
} matrix;

matrix *create_empty_matrix(int nrows, int ncols);
int get_value(matrix *m, int rownum, int colnum);
void set_value(matrix *m, int rownum, int colnum, int val);
matrix *create_matrix(char *filename, int nrows, int ncols);

/* Part 2 */
matrix *matrix_addition(matrix *a, matrix *b);
matrix *matrix_multiplication(matrix *a, matrix *b);

/* Part 3 */
void free_two_d_array(int **array, int nrows, int ncols);
void free_matrix(matrix *m);
char *represent_matrix(matrix *m);

/* empty line just in case! */
