#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
sem_t lock;
int count = 0;
int times = 90000;
void *producer(void *arg){
  int i;
  for (i = 0; i < times; i++)
  {
    sem_wait(&lock);  // Entry section
    count++;          // Crititcal section
    sem_post(&lock);  // Exit section
  }
  // Remainder section
  return NULL;
}

void *consumer(void *arg){
  int i;
  for (i = 0; i < times; i++)
  {
    sem_wait(&lock);  // Entry section
    count--;          // Critical section
    printf("%d\n",count);
    sem_post(&lock);  // Exit section
  }
  // Remainder Section
  return NULL;
}

int main(){
  sem_init(&lock, 0, 1);
  pthread_t p1, p2;
  printf("Initial count: %d\n", count);
  pthread_create(&p1, NULL, producer, NULL);
  pthread_create(&p2, NULL, consumer, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("Final count: %d\n", count);
  return 0;
}