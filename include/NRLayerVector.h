#include "NRLayer.h"

#ifndef TASK2_INCLUDE_NRLAYERVECTOR_H_
#define TASK2_INCLUDE_NRLAYERVECTOR_H_

typedef struct {
  NRLayer *mem;
  size_t size;
  size_t length;
} NRLayerVector;

int NRLayerVector_init(NRLayerVector *layer_vector);
int NRLayerVector_delete(NRLayerVector *layer_vector);

int NRLayerVector_push(NRLayerVector *layer_vector, NRLayer *layer);

#endif //TASK2_INCLUDE_NRLAYERVECTOR_H_
