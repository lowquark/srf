
#include "queue.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

void squeue_init(squeue_t * q, void ** elements, unsigned int max_size) {
  q->elements = elements;
  q->max_size = max_size;
  q->begin_idx = 0;
  q->size = 0;
}
void squeue_clear(squeue_t * q) {
  q->begin_idx = 0;
  q->size = 0;
}
int squeue_push(squeue_t * q, void * p) {
  if(q->size < q->max_size) {
    unsigned int dst_idx = q->begin_idx + q->size;
    if(dst_idx >= q->max_size) {
      dst_idx -= q->max_size;
    }
    q->elements[dst_idx] = p;
    q->size ++;
    return 1;
  } else {
    return 0;
  }
}
int squeue_pop(squeue_t * q, void ** p) {
  if(q->size > 0) {
    *p = q->elements[q->begin_idx];
    q->begin_idx ++;
    if(q->begin_idx == q->max_size) {
      q->begin_idx = 0;
    }
    q->size --;
    return 1;
  } else {
    return 0;
  }
}

void squeue_test() {
  void * data[16];
  squeue_t q;
  squeue_init(&q, data, 16);

  for(unsigned int j = 0 ; j < 100 ; j ++) {
    for(size_t i = 0 ; i < 16 ; i ++) {
      assert(squeue_push(&q, (void *)i) == 1);
    }
    assert(squeue_push(&q, (void *)0x00) == 0);

    void * p = NULL;
    for(size_t i = 0 ; i < 16 ; i ++) {
      assert(squeue_pop(&q, &p) == 1);
      assert(p == (void *)i);
    }
    assert(squeue_pop(&q, &p) == 0);

    assert(squeue_push(&q, (void *)0x00) == 1);
    assert(squeue_pop(&q, &p) == 1);
    assert(p == (void *)0x0);
  }
}

void queue_clear(queue_t * q) {
  free(q->elements);
  q->elements = NULL;
  q->max_size = 0;
  q->begin_idx = 0;
  q->size = 0;
}

void queue_push(queue_t * q, void * p) {
  if(q->max_size == 0) {
    q->max_size = 8;
    q->elements = malloc(q->max_size*sizeof(*q->elements));
    q->begin_idx = 0;
    q->size = 0;
  } else if(q->size == q->max_size) {
    unsigned int mid_idx = q->max_size;
    q->max_size *= 2;
    q->elements = realloc(q->elements, q->max_size*sizeof(*q->elements));

    // unroll
    for(unsigned int i = 0 ; i < q->begin_idx ; i ++) {
      q->elements[mid_idx + i] = q->elements[i];
    }
  }

  unsigned int dst_idx = q->begin_idx + q->size;
  if(dst_idx >= q->max_size) {
    dst_idx -= q->max_size;
  }
  q->elements[dst_idx] = p;
  q->size ++;
}
int queue_pop(queue_t * q, void ** p) {
  if(q->size > 0) {
    *p = q->elements[q->begin_idx];
    q->begin_idx ++;
    if(q->begin_idx == q->max_size) {
      q->begin_idx = 0;
    }
    q->size --;
    return 1;
  } else {
    return 0;
  }
}

void queue_test() {
  queue_t q = { 0 };

  for(unsigned int j = 2 ; j < 100 ; j ++) {
    for(size_t i = 0 ; i < j*2 ; i ++) {
      queue_push(&q, (void *)i);
    }

    void * p = NULL;
    for(size_t i = 0 ; i < j*2 ; i ++) {
      assert(queue_pop(&q, &p) == 1);
      assert(p == (void *)i);
    }
    assert(queue_pop(&q, &p) == 0);

    queue_push(&q, (void *)0x00);
    assert(queue_pop(&q, &p) == 1);
    assert(p == (void *)0x0);
  }

  queue_clear(&q);
}

