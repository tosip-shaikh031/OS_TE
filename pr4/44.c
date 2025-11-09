#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int flight_seats = 5;      
int reader_count = 0;

sem_t rw_mutex;
pthread_mutex_t mutex;

void* reader(void* arg);
void* writer(void* arg);

int main() {
    srand(time(NULL));

    pthread_t rtid[5], wtid[3];   // 5 readers, 3 writers
    int r_id[5], w_id[3];

    sem_init(&rw_mutex, 0, 1); 
    pthread_mutex_init(&mutex, NULL);
    // Create reader threads
    for (int i = 0; i < 5; i++) {
        r_id[i] = i + 1;
        pthread_create(&rtid[i], NULL, reader, (void*)&r_id[i]);
    }
    // Create writer threads
    for (int i = 0; i < 3; i++) {
        w_id[i] = i + 1;
        pthread_create(&wtid[i], NULL, writer, (void*)&w_id[i]);
    }
    // Join threads 
    for (int i = 0; i < 5; i++)
        pthread_join(rtid[i], NULL);

    for (int i = 0; i < 3; i++)
        pthread_join(wtid[i], NULL);

    sem_destroy(&rw_mutex);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* reader(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);
        reader_count++;
        if (reader_count == 1)
            sem_wait(&rw_mutex);  // First reader locks the resource
        pthread_mutex_unlock(&mutex);

        // Reading section
        printf("Reader %d is checking available seats: %d\n", id, flight_seats);
        usleep(rand() % 300000); // simulate reading delay

        pthread_mutex_lock(&mutex);
        reader_count--;
        if (reader_count == 0)
            sem_post(&rw_mutex);  // Last reader unlocks the resource
        pthread_mutex_unlock(&mutex);

        usleep(rand() % 500000); // simulate delay before reading again
    }

    pthread_exit(NULL);
}

void* writer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sem_wait(&rw_mutex);
        int seats_to_book = rand() % 2 + 1;
        if (flight_seats >= seats_to_book) {
            flight_seats -= seats_to_book;
            printf("Writer %d booked %d seat(s). Remaining seats: %d\n",
                   id, seats_to_book, flight_seats);
        } else {
            printf("Writer %d tried to book %d seat(s), but not enough seats available!\n",
                   id, seats_to_book);
        }

        usleep(rand() % 400000); // simulate writing time
        sem_post(&rw_mutex); // unlock resource

        usleep(rand() % 600000); // simulate delay before next write
    }

    pthread_exit(NULL);
}
