#include "ht1.h"
/*      ======== intVector functions ========       */

void intVectorEnlarge(intVector *old) {
  intVector new;
  new.array = realloc(old->array, 2 * old->size * sizeof(int));

  memchk(new.array);

  old->array = new.array;
  old->size *= 2;

}

void intVectorPrint(intVector *vector) {
  int i;
  for (i = 0; i < vector->takenSpots; i++)
    printf("%d, ", vector->array[i]);

}

void intVectorPush(intVector *vector, int num) {
  vector->takenSpots++;
  if (vector->takenSpots > vector->size)
    intVectorEnlarge(vector);

  vector->array[vector->takenSpots - 1] = num;

}


