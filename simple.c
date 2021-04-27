#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

//just a small test to observe the behaviour of the GenMC model checker
atomic_int data_;
atomic_bool ready_;

void *thread1(void *arg) {
  atomic_store_explicit(&data_, 42, memory_order_relaxed);
  atomic_store_explicit(&ready_, 1, memory_order_relaxed);
  return NULL;
}

void *thread2(void *arg) {
  if (atomic_load_explicit(&ready_, memory_order_acquire) == 1) {
	int r = atomic_load_explicit(&data_, memory_order_relaxed);
	//assert(r == 42);
  }
  return NULL;
}

int main() {

  pthread_t thr1, thr2;
  int res = pthread_create(&thr1, NULL, thread1, NULL);
  if (res != 0) {
	printf("error occured\n");
	return -1;
  }
  res = pthread_create(&thr2, NULL, thread2, NULL);
  if (res != 0) {
	printf("error occured -- sorry\n");
	return -1;
  }
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);

  return 0;
}