#include "../include/pyVector.h"
#include "../include/ht1.h"
#include <string.h>

/*      ======== pyVector functions ========        */

void vectorCopyString(pyVector *vector, int pos, char *buf) {
  if (vector->array[pos].name == NULL)
    vector->array[pos].name = malloc(strlen(buf) + 1);

  memchk(vector->array[pos].name);
  strcpy(vector->array[pos].name, buf);
}

void vectorEnlarge(pyVector *old) {
  pyVector new;

  new.array = realloc(old->array, 2 * old->size * sizeof(PYTHON_FUNC));

  memchk(new.array);

  old->array = new.array;
  old->size *= 2;

}

void vectorInit(pyVector *vector) {
  vector->array = malloc(ARR_SIZE * sizeof(PYTHON_FUNC));
  vector->size = ARR_SIZE;
  vector->takenSpots = 0;

  memchk(vector->array);

}

int vectorFindName(pyVector *vector, char *buf) {
  int i;
  for (i = 0; i < vector->takenSpots; i++)
    if (strcmp(vector->array[i].name, buf) == 0)
      return i;

  return -1;
}

void vectorFree(pyVector *vector) {
  int i;
  if (vector->array == NULL)
    return;

  for (i = 0; i < vector->takenSpots; i++) {
    if (vector->array[i].name != NULL)
      free(vector->array[i].name);
    intVectorFree((&vector->array[i].lines));
  }

  free(vector->array);

  vector->size = 0;
  vector->takenSpots = 0;
}

void vectorPrint(pyVector *vector) {
  int i;

  for (i = 0; i < vector->takenSpots; i++) {
    printf("%d. Function '%s' is present on lines ", i + 1, vector->array[i].name);
    intVectorPrint(&(vector->array[i].lines));
    printf(" with %d parameters\n", vector->array[i].n_parameters);
  }

}

void vectorPush(pyVector *vector, char *buf, int line) {
  int pos = vector->takenSpots;

  if (pos + 1 > vector->size)
    vectorEnlarge(vector);

  vector->takenSpots++;

  vector->array[pos].name = NULL;
  vectorCopyString(vector, pos, buf);

  intVectorInit(&(vector->array[pos].lines));

  vector->array[pos].n_parameters = 0;

  memchk(vector->array[pos].lines.array);

  intVectorPush(&(vector->array[pos].lines), line);

}