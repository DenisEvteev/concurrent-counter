#include "counter.h"
#include <pthread.h>

//contention-aware counter
counter cnt;
//atomic counter of inc performed
atomic_int number;

static const size_t INC = 3;
static const size_t GET = 2;

/*
 * The test creates INC threads performing inc on the global counter
 * GET threads getting the value from global counter
 *
 * We strive to check the correctness of the algorithm via GenMC tool to prove that
 * this implementation is linearizable
 * */

void *do_inc(void *arg);
void *do_get(void *arg);

int main() {
  pthread_t workers[INC + GET];
  for (size_t i = 0; i < INC; ++i) {
	if (pthread_create(workers + i, NULL, do_inc, NULL) != 0) {
	  printf("error in creating an inc thread\n");
	  abort();
	}
  }

  for (size_t i = INC; i < INC + GET; ++i) {
	if (pthread_create(workers + i, NULL, do_get, NULL) != 0) {
	  printf("error in creating a get thread\n");
	  abort();
	}
  }

  for (size_t i = 0; i < INC + GET; ++i) {
	if (pthread_join(workers[i], NULL) != 0) {
	  printf("error in joining a thread\n");
	  abort();
	}
  }
  assert(number == INC);
  return 0;
}

void *do_inc(void *arg) {
  atomic_fetch_add_explicit(&number, 1, memory_order_release);
  inc(&cnt);
  return NULL;
}

void *do_get(void *arg) {
  int res = get(&cnt);
  assert(res <= atomic_load_explicit(&number, memory_order_acquire));
  return NULL;
}