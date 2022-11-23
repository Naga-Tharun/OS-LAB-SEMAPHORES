#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (num_of_philosopher + 4) % N
#define RIGHT (num_of_philosopher + 1) % N

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

// sem_t is a typedef defined in the header file as (apparently) some variety of integer.
sem_t mutex;
sem_t S[N];

void test(int num_of_philosopher)
{
		if (state[num_of_philosopher] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) 
		{
			// state that eating
			state[num_of_philosopher] = EATING;

			sleep(2);

			printf("Philosopher %d takes fork %d and %d\n", num_of_philosopher + 1, LEFT + 1, num_of_philosopher + 1);

			printf("Philosopher %d is Eating\n", num_of_philosopher + 1);

			/* sem_post(&S[num_of_philosopher]) has no effect
  			during takefork
  			used to wake up hungry philosophers
  			during putfork */
			sem_post(&S[num_of_philosopher]);
	}
}

// take up Forks
void take_fork(int num_of_philosopher)
{

	sem_wait(&mutex);

	// state that hungry
	state[num_of_philosopher] = HUNGRY;

	printf("Philosopher %d is Hungry\n", num_of_philosopher + 1);

	// eat if neighbours are not eating
	test(num_of_philosopher);

	sem_post(&mutex);

	// if unable to eat wait to be signalled
	sem_wait(&S[num_of_philosopher]);

	sleep(1);
}

// put down Forks
void put_fork(int num_of_philosopher)
{

	sem_wait(&mutex);

	// state that thinking
	state[num_of_philosopher] = THINKING;

	printf("Philosopher %d putting fork %d and %d down\n",num_of_philosopher + 1, LEFT + 1, num_of_philosopher + 1);
	printf("Philosopher %d is thinking\n", num_of_philosopher + 1);

	test(LEFT);
	test(RIGHT);

	sem_post(&mutex);
}

void* philosopher(void* num)
{

	while (1) 
	{
		int* i = num;

		sleep(1);

		take_fork(*i);

		sleep(0);

		put_fork(*i);
	}
}

int main()
{
	int i;
	pthread_t thread_id[N];

	// initialize the semaphores
	sem_init(&mutex, 0, 1);

	for (i = 0; i < N; i++)

	sem_init(&S[i], 0, 0);

	for (i = 0; i < N; i++) {

	// create philosopher processes
	pthread_create(&thread_id[i], NULL,philosopher, &phil[i]);

	printf("Philosopher %d is thinking\n", i + 1);
}

	for (i = 0; i < N; i++)
	{
		pthread_join(thread_id[i], NULL);
	}
}