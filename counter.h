#pragma once
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#ifdef PORTABLE_RANDOM_GENERATOR
size_t rnd_value(size_t max); //max should be less than or equal to 66
static const int start = 42;
static const int add = 24;
atomic_int seed = 5;
#else
_Thread_local int delta_ = 0;
#endif

#define SIZE 17

typedef struct cnt {
  atomic_int array[SIZE];
} counter;

void inc(counter *c) {
  size_t i;
#ifdef PORTABLE_RANDOM_GENERATOR
  i = rnd_value(SIZE);
#else
  i = (pthread_self() + delta_++) % SIZE;
#endif

#ifdef DEBUG
  printf("random value in thread#%ld equals to %ld\n", pthread_self(), i);
#endif
  atomic_fetch_add_explicit(c->array + i, 1, memory_order_release);
}

int get(counter *c) {
  int sum = 0;
  for (int i = 0; i < SIZE; ++i) {
	sum += atomic_load_explicit(c->array + i, memory_order_acquire);
  }
  return sum;
}

#ifdef PORTABLE_RANDOM_GENERATOR
size_t rnd_value(size_t max) {
  assert(max != 0);
  atomic_store_explicit(&seed, (start * atomic_load_explicit(&seed, memory_order_relaxed) + add),
						memory_order_relaxed);
  return seed % max;
}
#endif