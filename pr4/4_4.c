/* 
A real-world example of the readers-writers problem is an airline reservation system: 
• Readers: want to read flight information 
• Writers: want to make flight reservations 
• Potential problem: if readers and writers can access the shared data simultaneously then 
readers/writers may view flights as being available when they’ve actually just been booked. 
Implement the solution using mutex and semaphore that controls access to the reader count and 
database given the priority to readers over writers. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Semaphores and mutex
sem_t db;               // Controls access to shared data (database)
pthread_mutex_t mutex;  // Controls access to read_count variable

int read_count = 0;     // Number of readers currently accessing data
int flight_seats = 5;   // Shared resource — represents available seats

// Reader thread function
void* reader(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&db);  // first reader locks the database for writers
        pthread_mutex_unlock(&mutex);

        // Reading section
        printf("Reader %d: checking available seats = %d\n", id, flight_seats);
        sleep(1); // simulate read time

        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&db); // last reader unlocks the database
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 1); // wait before reading again
    }
    return NULL;
}

// Writer thread function
void* writer(void* arg) {
    int id = *((int*)arg);
    while (1) {
        sem_wait(&db); // only one writer can access at a time

        // Writing section (booking a seat)
        if (flight_seats > 0) {
            printf("Writer %d: booking a seat... (before = %d)\n", id, flight_seats);
            flight_seats--;
            printf("Writer %d: seat booked! (after = %d)\n", id, flight_seats);
        } else {
            printf("Writer %d: no seats available to book.\n", id);
        }
        sleep(1); // simulate write time

        sem_post(&db); // release database

        sleep(rand() % 5 + 1); // wait before next attempt
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t readers[3], writers[2]; // 3 readers and 2 writers
    int r_id[3] = {1, 2, 3};
    int w_id[2] = {1, 2};

    // Initialize synchronization primitives
    sem_init(&db, 0, 1); // binary semaphore for database access
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < 3; i++)
        pthread_create(&readers[i], NULL, reader, &r_id[i]);

    for (int i = 0; i < 2; i++)
        pthread_create(&writers[i], NULL, writer, &w_id[i]);

    // Join threads (infinite loop, so won't really join)
    for (int i = 0; i < 3; i++)
        pthread_join(readers[i], NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);

    // Cleanup
    sem_destroy(&db);
    pthread_mutex_destroy(&mutex);

    return 0;
}
