#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define FILE_NAME "data.txt"

sem_t empty;   // Indicates producer can write
sem_t full;    // Indicates consumer can read
pthread_mutex_t mutex;

void* producer(void* arg);
void* consumer(void* arg);

int main() {
    srand(time(NULL));

    pthread_t prod_thread, cons_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, 1); // File initially empty, so producer can write
    sem_init(&full, 0, 0);  // No data available initially
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads 
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* producer(void* arg) {
    while (1) {
        sem_wait(&empty);             
        pthread_mutex_lock(&mutex);

        int num = rand() % 10;

        FILE* fptr = fopen(FILE_NAME, "w");
        if (fptr == NULL) {
            perror("File open error");
            exit(1);
        }

        fprintf(fptr, "%d", num);
        fclose(fptr);

        printf("🍳 Producer wrote %d to file.\n", num);

        pthread_mutex_unlock(&mutex); 
        sem_post(&full);              

        int sleep_time = rand() % 100000; // microseconds
        usleep(sleep_time); 
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);       
        pthread_mutex_lock(&mutex);   

        FILE* fptr = fopen(FILE_NAME, "r");
        if (fptr == NULL) {
            perror("File open error");
            exit(1);
        }

        int num;
        fscanf(fptr, "%d", &num);
        fclose(fptr);

        printf("🍽️  Consumer read %d from file.\n", num);

        pthread_mutex_unlock(&mutex); 
        sem_post(&empty);            

        usleep(50000); // 50 ms delay for readability
    }
    return NULL;
}
