#include "vector.h"

int Vector_init(Vector *vector, unsigned len) {
  if (vector == NULL)
    return -1;

  vector->length = len;
  vector->mem    = malloc(len * sizeof(double));

  if(vector->mem == NULL)
    return -1;

  return 0;
}

int Vector_delete(Vector *vector) {
  if(vector == NULL)
    return -1;

  vector->length = 0;
  free(vector->mem);

  vector->mem = NULL;

return 0;
}

int Vector_fill_from_stream(Vector* vector, FILE* stream){
  int i = 0;
  int error = 0;

  if(vector == NULL || stream == NULL)
    return -1;

  if(vector->mem == NULL)
    return -1;

  for (; i < vector->length; ++i)
    if(fscanf(stream, "%lf", &(vector->mem[i])) == EOF)
      error = -1;

  return error;
}


