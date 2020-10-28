#ifndef TASK2_SRC_MATRIX_VECTOR_H_
#define TASK2_SRC_MATRIX_VECTOR_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct{
  double *mem;
  size_t length;
} Vector;

int Vector_init(Vector* vector, unsigned len);
int Vector_delete(Vector* vector);

int Vector_fill_from_stream(Vector* vector, FILE* stream);

#endif //TASK2_SRC_MATRIX_VECTOR_H_
