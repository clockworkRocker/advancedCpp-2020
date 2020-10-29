#include "NRProcess.h"

int NR_process_linear(Matrix* input, NRLayerVector *description, FILE *output){
  int error = 0;
  int i;

  if (input == NULL || description == NULL || output == NULL)
    return -1;

  for (i = 0; i < description->length && error == 0; ++i){
    if(NRLayer_activate(description->mem+i, input) < 0)
      error = -1;
  }

  Matrix_print(input, output);

  return error;
}
