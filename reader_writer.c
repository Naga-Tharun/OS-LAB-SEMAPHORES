#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

// defining READERS = 4 and WRITERS = 2 according to problem
#define READERS 4
#define WRITERS 2

// binary semaphore used by writers and readers
sem_t db;
// binary semaphore used by readers
pthread_mutex_t mutex;

// sample data of the database used by readers and writers
int count = 1;
// count of the readers reading the database
int reader_count = 0;

void *writer(void *writer_no){
    // if db is 1 then it becomes 0 and writer can access the database else writer waits
    sem_wait(&db);
    // writer modifying the database
    count = count*2;
    printf("Writer %d modified count to %d\n",(*((int *)writer_no)),count);
    // making db back to 1 after the writer left
    sem_post(&db);
}

void *reader(void *reader_no){   
    // reducing mutex before modifying reader_count
    pthread_mutex_lock(&mutex);
    reader_count++;
    // if a reader exist in database then block the writers from accessing database
    if(reader_count == 1){
        sem_wait(&db); 
    }
    // releasing the mutex
    pthread_mutex_unlock(&mutex);

    // reading section
    printf("Reader %d: read count as %d\n",*((int *)reader_no),count);

    // reducing mutex for modifying reader_count when reader job is done
    pthread_mutex_lock(&mutex);
    reader_count--;
    // if there are no readers then writer can be allowed
    if(reader_count == 0){
        sem_post(&db);
    }
    // relasing the mutex after reader_count updation
    pthread_mutex_unlock(&mutex);
}

int main(){   

    pthread_t read[READERS],write[WRITERS];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&db,0,1);

    // used for numbering writers and readers
    int a[4] = {1,2,3,4};

    // creating the reader and writer processes randomly
    pthread_create(&read[0], NULL, (void *)reader, (void *)&a[0]);
    pthread_create(&read[1], NULL, (void *)reader, (void *)&a[1]);
    pthread_create(&write[0], NULL, (void *)writer, (void *)&a[0]);
    pthread_create(&read[2], NULL, (void *)reader, (void *)&a[2]);
    pthread_create(&write[1], NULL, (void *)writer, (void *)&a[1]);
    pthread_create(&read[3], NULL, (void *)reader, (void *)&a[3]);

    for(int i = 0; i < READERS; i++){
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < WRITERS; i++){
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&db);
    return 0;
}