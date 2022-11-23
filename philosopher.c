#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// defining the number of philosophers to 5
#define N 5
// variables used for knowing the state of the philosopher
#define THINKING 2
#define HUNGRY 1
#define EATING 0
// used for checking the availablity of forks on table by checking stae of adjacent philosophers
#define LEFT (num_of_philosopher + 4) % N
#define RIGHT (num_of_philosopher + 1) % N

// stores the state of philosopher
int state[N];
// ids of the philosophers
int phil[N] = {0, 1, 2, 3, 4};

// binary semaphore for preventing collision fo mutiple operations
sem_t mutex;
// binary semaphore used for each philosopher
sem_t S[N];

void test(int num_of_philosopher){
	// if philospher state is hungry and the left and right forks are available by checking state of adjacent philosophers
	if(state[num_of_philosopher]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING){
		// update state to eating
		state[num_of_philosopher] = EATING;
		sleep(2);
		// printing the state of philosopher
		printf("Philosopher %d takes fork %d and %d\n", num_of_philosopher+1, LEFT+1, num_of_philosopher+1);
		printf("Philosopher %d is Eating\n", num_of_philosopher + 1);

		/* sem_post(&S[num_of_philosopher]) has no effect
		during takefork
		used to wake up hungry philosophers
		during putfork */
		sem_post(&S[num_of_philosopher]);
	}
}

void take_fork(int num_of_philosopher){
	// mutex is reduced to prevent processes interfering with each others shared data
	sem_wait(&mutex);

	// change the philosopher state from thinking to hungry
	state[num_of_philosopher] = HUNGRY;
	printf("Philosopher %d is Hungry\n", num_of_philosopher + 1);

	// if the neighour is eating then wait till he changes his state
	test(num_of_philosopher);
	// releases the mutex after completion of the process
	sem_post(&mutex);

	// if unable to eat then wait till s changes
	sem_wait(&S[num_of_philosopher]);
	sleep(1);
}

void put_fork(int num_of_philosopher){
	// mutex is reduced to prevent processes interfering with each others shared data
	sem_wait(&mutex);

	// change the state of philsopher to thinking
	state[num_of_philosopher] = THINKING;
	printf("Philosopher %d putting fork %d and %d down\n",num_of_philosopher + 1, LEFT + 1, num_of_philosopher + 1);
	printf("Philosopher %d is thinking\n", num_of_philosopher + 1);

	// allows the adjacent philosophers to check if they have the forks or resources to change their state
	test(LEFT);
	test(RIGHT);
	// releases the mutex after completion of the process
	sem_post(&mutex);
}

void* philosopher(void* num){
	while(1){
		// i is the philosopher id
		int* i = num;
		sleep(1);
		// philosopher takes the  fork if available else wait
		take_fork(*i);
		sleep(0);
		// philospher puts the fork after eating
		put_fork(*i);
	}
}

int main(){
	int i;
	pthread_t thread_id[N];

	// initialize the semaphores
	sem_init(&mutex, 0, 1);
	for(i=0; i<N; i++){
		sem_init(&S[i], 0, 0);
	}
	// creating philosopher processes
	for(i=0; i<N; i++){
		pthread_create(&thread_id[i], NULL,philosopher, &phil[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}

	for(i=0; i<N; i++){
		pthread_join(thread_id[i], NULL);
	}
}