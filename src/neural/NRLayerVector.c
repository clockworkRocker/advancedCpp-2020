#include "NRLayerVector.h"

/* ------------ "CONSTRUCTOR" ------------ */
int NRLayerVector_init(NRLayerVector *layer_vector) {
  const int INITIAL_SIZE = 2;

  if (layer_vector == NULL)
    return -1;

  layer_vector->mem = malloc(INITIAL_SIZE * sizeof(NRLayer));
  if (layer_vector->mem == NULL)
    return -1;

  layer_vector->length = 0;
  layer_vector->size = INITIAL_SIZE;

  return 0;
}

/* ------------ "DESTRUCTOR" ------------ */
int NRLayerVector_delete(NRLayerVector *layer_vector) {
  if (layer_vector == NULL)
    return 0;

  int i;
  for (i = 0; i < layer_vector->length; ++i)
    NRLayer_delete(layer_vector->mem + i);

  free(layer_vector->mem);

  layer_vector->mem = NULL;
  layer_vector->size = 0;
  layer_vector->length = 0;

  return 0;
}

/* ------------ ENLARGEMENT FUNCTION ------------ */
int NRLayerVector_enlarge(NRLayerVector *layer_vector) {
  if(layer_vector == NULL)
    return -1;

  const unsigned enlarge_multiplier = 2;

  NRLayer *tmp = realloc(layer_vector->mem, enlarge_multiplier * layer_vector->size * sizeof(NRLayer));
  if (tmp == NULL)
    return -1;

  layer_vector->mem = tmp;
  layer_vector->size *= enlarge_multiplier;

  return 0;
}

/* ------------ PUSH ------------ */
int NRLayerVector_push(NRLayerVector *layer_vector, NRLayer *layer) {
  if (layer_vector == NULL || layer == NULL)
    return -1;

  if (layer_vector->length == layer_vector->size)
    if (NRLayerVector_enlarge(layer_vector) < 0)
      return -1;

  if(NRLayer_init(layer_vector->mem + layer_vector->length, layer->name, layer->type, layer->input_dim, layer->output_dim) < 0)
    return -1;

  layer_vector->length++;

  return 0;
}