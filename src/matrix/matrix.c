#include "matrix.h"

/* ------------ "CONSTRUCTOR" ------------ */
int Matrix_init(Matrix *matrix, int len, int wid) {

  int i = 0;
  int error = 0;

  if (matrix == NULL)
    return -1;

  if (wid < 0 || len < 0)
    return -1;

  matrix->length = len;
  matrix->width = wid;
  matrix->cell = malloc(wid * sizeof(double *));

  if (matrix->cell == NULL)
    return -1;

  for (; i < wid; ++i) {
    matrix->cell[i] = malloc(len * sizeof(double));
    if (matrix->cell[i] == NULL)
      error = -1;
  }

  return error;
}

/* ------------ "DESTRUCTOR" ------------ */
int Matrix_delete(Matrix *matrix) {
  int i = 0;

  if (matrix == NULL)
    return -1;
  if(matrix->cell == NULL)
    return -1;

  for (; i < matrix->width; ++i) {
    free(matrix->cell[i]);
  }

  free(matrix->cell);
  matrix->cell = NULL;
  matrix->width = 0;
  matrix->length = 0;

  return 0;
}

/* ------------ NULLIFICATION ------------ */
int Matrix_nullify(Matrix *matrix) {
  if (matrix == NULL)
    return -1;
  if (matrix->cell == NULL)
    return -1;

  int i, j;
  for (i = 0; i < matrix->width; ++i)
    for (j = 0; j < matrix->length; ++j)
      matrix->cell[i][j] = 0.;

  return 0;
}

/* ------------ ADDITION ------------ */
int Matrix_add(Matrix *lhs, Matrix *rhs) {
  register int i = 0, j;
  int limits[2];

  if (lhs == NULL || rhs == NULL)               // 1. Check arguments
    return -1;

  if (lhs->cell == NULL || rhs->cell == NULL)
    return -1;

  if (lhs->length < rhs->length)                // 2. Calculate addition limits
    limits[0] = lhs->length;
  else
    limits[0] = rhs->length;

  if (lhs->width < rhs->width)
    limits[1] = lhs->width;
  else
    limits[1] = rhs->width;

  for (; i < limits[1]; ++i) {                  // 3. Add
    for (j = 0; j < limits[0]; ++j)
      lhs->cell[i][j] += rhs->cell[i][j];
  }

  return 0;
}

int Matrix_add_Vector(Matrix *lhs, Vector *rhs) {
  if (lhs == NULL || rhs == NULL)
    return -1;

  if (lhs->cell == NULL || rhs->mem == NULL)
    return -1;

  if (rhs->length != lhs->length)
    return -1;

  int i, j;

  for (i = 0; i < lhs->width; ++i)
    for (j = 0; j < lhs->length; ++j)
      lhs->cell[i][j] += rhs->mem[j];

  return 0;
}

/* ------------ MULTIPLICATION ------------ */
int Matrix_multiply(Matrix *lhs, Matrix *rhs, Matrix *result) {

  if (lhs == NULL || rhs == NULL || result == NULL)
    return -1;
  if (lhs->cell == NULL || rhs->cell == NULL)
    return -1;
  if (result->cell == NULL) {
    Matrix_init(result, rhs->length, lhs->width);
    Matrix_nullify(result);
  }

  if (lhs->length != rhs->width)
    return -1;

  int i, j, k;
  for (i = 0; i < lhs->width; ++i)
    for (j = 0; j < rhs->length; ++j)
      for (k = 0; k < rhs->width; ++k)
        result->cell[i][j] += lhs->cell[i][k] * rhs->cell[k][j];

  return 0;
}

/* ------------ INPUT ------------ */
int Matrix_fill_from_stream(Matrix *matrix, FILE *stream) {
  int i = 0, j;
  int error = 0;

  if (matrix == NULL || stream == NULL)
    return -1;

  if (matrix->cell == NULL)
    return -1;

  if(Matrix_nullify(matrix) < 0)
    return -1;

  for (; i < matrix->width && error == 0; ++i) {
    for (j = 0; j < matrix->length && error == 0; ++j)
        if(fscanf(stream, "%lf", &(matrix->cell[i][j])) != 1)
          error = -1;
  }
  return error;
}

/* ------------ OUTPUT ------------ */
int Matrix_print(Matrix *matrix, FILE *stream) {
  int i, j;

  if (matrix == NULL || stream == NULL)
    return -1;
  if (matrix->cell == NULL)
    return -1;

  for (i = 0; i < matrix->width; ++i) {
    for (j = 0; j < matrix->length; ++j)
      fprintf(stream, "%lf ", matrix->cell[i][j]);
    fputc('\n', stream);
  }

  return 0;
}
