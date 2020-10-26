#include <stdio.h>
#include <stdlib.h>
#include "intVector.h"
#include "pyVector.h"

/*
    ALGORHYTHM
    1. Open file
    2. Read it symbol by symbol
    3. Save symbols to  buffer
    4. When size is 3, compare the buffer to "def"
        4.1 If true, create a new structure for the array
        4.2 Raise the flag that what comes next is a function definition and save its line number
    5. On ' ' reset the buffer
    6. After every reset check if we are looking at a function
        6.1 If we are then if we encounter '(', raise an "inside parameters" flag and copy the buffer into the structure in order to save the name
        6.2 Read symbols until we encounter a ',' or ':'
            6.2.1. Raise the n_parameters count; reset buffer
        6.3. If we encounter a ')', drop the "inside parameters" flag; reset buffer
        6.4. If we encounter a ':' while not inside parameters drop the 'inside function' check
    7. If we encounter a '\n' increment the line counter
	8. On 'EOF' close the file, output the structure array and clear it afterwards.
*/

#define BUF_SIZE 128
#define ARR_SIZE 4

/*Main functions*/
void collectPyFunctionsFromFile(FILE*, pyVector*);

/*Utility functions*/
char memchk(void*);
int checkForIdentifier(const char*,int);
void bufReset(int*, char*);

/*intVector manipulation functions*/
void intVectorEnlarge(intVector*);
void intVectorFree(intVector*);
void intVectorInit(intVector*);
void intVectorPrint(intVector*);
void intVectorPush(intVector*, int);

/*pyVector manipulation functions*/
void vectorCopyString(pyVector*, int, char*);
void vectorEnlarge(pyVector*);
void vectorInit(pyVector*);
int vectorFindName(pyVector*, char*);
void vectorFree(pyVector*);
void vectorPrint(pyVector*);
void vectorPush(pyVector*, char*, int);