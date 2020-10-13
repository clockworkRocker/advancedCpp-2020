//
// Created by alexander on 13.10.2020.
//

#include "intVector.h"

#ifndef TASK1_INCLUDE_PYVECTOR_H_
#define TASK1_INCLUDE_PYVECTOR_H_

typedef struct pf {
  char* name;
  intVector lines;
  int n_parameters;
} PYTHON_FUNC;

typedef struct pv{
  PYTHON_FUNC* array;
  int size;
  int takenSpots;
} pyVector;

#endif //TASK1_INCLUDE_PYVECTOR_H_
