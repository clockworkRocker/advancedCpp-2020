#include <pthread.h>
#include <sys/sysinfo.h>
#include "NRProcess.h"

struct thread_job_info {
  Matrix *matrix;
  NRLayerVector *description;
};

int describe_job(struct thread_job_info *TJI,
                 Matrix *original,
                 NRLayerVector *description,
                 int n_job,
                 int starting_point,
                 int batch_size) {

  Matrix *piece = malloc(sizeof(Matrix));
  Matrix_init(piece, original->length, batch_size);

  int i, j;

  for (i = 0; i < piece->width; ++i)
    for (j = 0; j < piece->length; ++j)
      piece->cell[i][j] = original->cell[starting_point + i][j];

  TJI[n_job].matrix = piece;
  TJI[n_job].description = description;

  return 0;
}

void *job(void *param) {
  int i;
  int error = 0;
  struct thread_job_info *TJI = (struct thread_job_info *) param;

  for (i = 0; i < TJI->description->length && error == 0; ++i) {
    if (NRLayer_activate(TJI->description->mem + i, TJI->matrix) < 0)
      error = -1;
  }

  pthread_exit(&error);
}

int NR_process_parallel(Matrix *matrix, NRLayerVector *description, FILE *output_file) {
  const int CORE_COUNT = get_nprocs();

  struct thread_job_info TJI[CORE_COUNT];

  if (matrix == NULL || description == NULL || output_file == NULL)
    return -1;

  pthread_t workers[CORE_COUNT];
  size_t batch_size = matrix->width / CORE_COUNT + 1;

  int i = 0;
  for (; i < CORE_COUNT; ++i) {
    if (i == CORE_COUNT - 1)
      describe_job(TJI, matrix, description, i, i * batch_size, matrix->width % batch_size);
    else
      describe_job(TJI, matrix, description, i, i * batch_size, batch_size);

    pthread_create(&workers[i], NULL, job, TJI + i);
  }

  for (i = 0; i < CORE_COUNT; ++i) {
    pthread_join(workers[i], NULL);
  }

  for (i = 0; i < CORE_COUNT; ++i) {
    Matrix_print(TJI[i].matrix, output_file);
    fputc('\n', output_file);
    Matrix_delete(TJI[i].matrix);
    free(TJI[i].matrix);
  }

  return 0;
}
