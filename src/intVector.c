#include "../include/ht1.h"
/*      ======== intVector functions ========       */

void intVectorEnlarge(intVector *old) {
  intVector new;
  new.array = realloc(old->array, 2 * old->size * sizeof(int));

  memchk(new.array);

  old->array = new.array;
  old->size *= 2;

}

void intVectorFree(intVector *vector){
  if(vector->array != NULL)
    free(vector->array);

  vector->size = 0;

  vector->takenSpots = 0;

}

void intVectorInit(intVector *vector) {
  vector->takenSpots = 0;

  vector->array = malloc(ARR_SIZE * sizeof(int));
  memchk(vector->array);

  vector->size = ARR_SIZE;
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


