#ifndef TASK2_INCLUDE_NRPROCESS_H_
#define TASK2_INCLUDE_NRPROCESS_H_
#include "NRLayerVector.h"

int NR_create_architecture(FILE *stream, const char *path, NRLayerVector *description);

int NR_process_linear(Matrix* input, NRLayerVector *description, FILE *output);
int NR_process_parallel(Matrix* input, NRLayerVector *description, FILE *output); //TODO Can try to implement parallel realisation

#endif //TASK2_INCLUDE_NRPROCESS_H_
