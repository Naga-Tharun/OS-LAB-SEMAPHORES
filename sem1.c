#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;
int times = 7500;

void *producer(void *arg)
{
  int i;
  for (i = 0; i < times; i++)
  {
    // Entry section
    count++;    // Critical section
    // Exit section
  }
  // Remainder section
  return NULL;
}

void *consumer(void *arg)
{
  int i;
  for (i = 0; i < times; i++)
  {
    // Entry section
    count--;    // Critical section
    // Exit section
  }
  // Remainder section
  return NULL;
}

int main()
{
  pthread_t p1, p2;
  printf("Initial count: %d\n", count);
  pthread_create(&p1, NULL, producer, NULL);
  pthread_create(&p2, NULL, consumer, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("Final count: %d\n", count);
  return 0;
}