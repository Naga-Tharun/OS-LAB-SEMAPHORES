#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
sem_t lock;
sem_t full;
sem_t empty;
int count = 0;
int times = 90000;
int buffer_size = 100;
void *producer(void *arg)
{
  int i = 0;
  while (i < times)
  {
    sem_wait(&empty);
    sem_wait(&lock); // Entry section
    count++;         // Crititcal section
    sem_post(&lock); // Exit section
    sem_post(&full);
  }
  // Remainder section
  i++;
  return NULL;
}

void *consumer(void *arg)
{
  int i = 0;
  while (i < times)
  {
    sem_wait(&full);
    sem_wait(&lock); // Entry section
    count--;         // Critical section
    sem_post(&lock); // Exit section
    sem_post(&empty);
  }
  // Remainder Section
  i++;
  return NULL;
}

int main()
{
  sem_init(&lock, 0, 1);
  sem_init(&empty, 0, buffer_size);
  sem_init(&full, 0, 0);
  pthread_t p1, p2;
  printf("Initial count: %d\n", count);
  pthread_create(&p1, NULL, producer, NULL);
  pthread_create(&p2, NULL, consumer, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("Final count: %d\n", count);
  return 0;
}