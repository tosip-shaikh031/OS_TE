#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>   

#define MAX_PIZZAS 5  

int pizza_counter[MAX_PIZZAS];
int count = 0;

sem_t empty;  // counts empty slots
sem_t full;   // counts filled slots
pthread_mutex_t mutex;

void* producer(void* arg) {
    int pizza_id = 1;
    while (1) {
        sem_wait(&empty);             
        pthread_mutex_lock(&mutex);   

        // Produce (add pizza to counter)
        pizza_counter[count] = pizza_id;
        printf("Mr. Simpson baked Pizza #%d and placed it on the counter. (Total on counter: %d)\n",
               pizza_id, count + 1);
        pizza_id++;
        count++;

        pthread_mutex_unlock(&mutex);  
        sem_post(&full);               

        sleep(rand() % 3 + 1);         
    }
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);              
        pthread_mutex_lock(&mutex); 

        // Consume (remove pizza from counter)
        int pizza = pizza_counter[count - 1];
        printf("Joey Tribbiani ate Pizza #%d. (Pizzas left on counter: %d)\n",
               pizza, count - 1);
        count--;

        pthread_mutex_unlock(&mutex);  
        sem_post(&empty);              

        sleep(rand() % 4 + 1);         
    }
}

int main() {
    pthread_t simpson_thread, joey_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, MAX_PIZZAS); // Initially all slots are empty
    sem_init(&full, 0, 0);           // Initially no pizzas are ready
    pthread_mutex_init(&mutex, NULL);

    printf("Producer-Consumer Problem (Mr. Simpson & Joey Tribbiani)\n");
    printf("----------------------------------------------------------\n");

    // Create threads
    pthread_create(&simpson_thread, NULL, producer, NULL);
    pthread_create(&joey_thread, NULL, consumer, NULL);

    // Join threads
    pthread_join(simpson_thread, NULL);
    pthread_join(joey_thread, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}