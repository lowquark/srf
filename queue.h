#ifndef QUEUE_H
#define QUEUE_H

typedef struct squeue {
  void ** elements;
  unsigned int max_size;
  unsigned int begin_idx;
  unsigned int size;
} squeue_t;

void squeue_init (squeue_t * q, void ** elements, unsigned int max_size);
void squeue_clear(squeue_t * q);

int  squeue_push(squeue_t * q, void * p);
int  squeue_pop (squeue_t * q, void ** p);

void squeue_test();

typedef struct queue {
  void ** elements;
  unsigned int max_size;
  unsigned int begin_idx;
  unsigned int size;
} queue_t;

void queue_clear(queue_t * q);

void queue_push(queue_t * q, void * p);
int  queue_pop (queue_t * q, void ** p);

void queue_test();

#endif
