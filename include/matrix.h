#ifndef TASK2_INCLUDE_MATRIX_H_
#define TASK2_INCLUDE_MATRIX_H_
#include "vector.h"

typedef struct m{
  double** cell;
  size_t width;
  size_t length;
} Matrix;

int Matrix_init(Matrix* matrix, int len, int wid);
int Matrix_delete(Matrix* matrix);

int Matrix_nullify(Matrix* matrix);

int Matrix_add(Matrix* lhs, Matrix* rhs);
int Matrix_add_Vector(Matrix* lhs, Vector* rhs);
int Matrix_multiply(Matrix* lhs, Matrix* rhs, Matrix* result);

int Matrix_fill_from_stream(Matrix* matrix, FILE* stream);
int Matrix_print(Matrix* matrix, FILE* stream);

#endif //TASK2_INCLUDE_MATRIX_H_
