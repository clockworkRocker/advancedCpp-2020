#ifndef TASK2_INCLUDE_NRLAYER_H_
#define TASK2_INCLUDE_NRLAYER_H_
#include "matrix.h"

#define NR_DENSE 0
#define NR_RELU 1

typedef struct {
  char *name;
  int type;
  size_t input_dim;
  size_t output_dim;
} NRLayer;

int NRLayer_init(NRLayer *dest, const char *name, int type, size_t input_dim, size_t output_dim);
int NRLayer_delete(NRLayer *dest);

int NRLayer_activate(NRLayer *description, Matrix *input);

#endif //TASK2_INCLUDE_NRLAYER_H_
