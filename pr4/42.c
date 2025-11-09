#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_CHAIRS 3      // number of chairs in hallway
#define NUM_STUDENTS 10   // number of students to simulate
int waitingStudents = 0;
sem_t students;     // students waiting for TA
sem_t ta;           // TA ready to help
pthread_mutex_t mutex;
void* student(void* id);
void* teaching_assistant(void* arg);
void* student(void* id) {
    int student_id = *(int*)id;
    while (1) {
        printf("Student %d is programming and may need help later.\n", student_id);
        sleep(rand() % 5 + 1);  // random time before seeking help
        pthread_mutex_lock(&mutex);
        if (waitingStudents < NUM_CHAIRS) {
            waitingStudents++;
            printf("Student %d is waiting. Waiting students = %d.\n",
                   student_id, waitingStudents);
            sem_post(&students);
            pthread_mutex_unlock(&mutex);
            sem_wait(&ta);
            printf("Student %d is getting help from TA.\n", student_id);
            sleep(rand() % 3 + 1); // getting help
            printf("Student %d is done with TA and going back to programming.\n", student_id);
        } else {
            printf("No chair available. Student %d will come back later.\n", student_id);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}
void* teaching_assistant(void* arg) {
    while (1) {
        sem_wait(&students);
        pthread_mutex_lock(&mutex);
        waitingStudents--;
        printf("TA is helping a student. Waiting students = %d.\n", waitingStudents);
        pthread_mutex_unlock(&mutex);
        sem_post(&ta);
        sleep(rand() % 3 + 1);
        printf("TA finished helping a student.\n");
    }
    return NULL;
}
int main() {
    srand(time(NULL));
    pthread_t taThread;
    pthread_t studentThreads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&taThread, NULL, teaching_assistant, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&studentThreads[i], NULL, student, &student_ids[i]);
    }
    pthread_join(taThread, NULL);
    // for (int i = 0; i < NUM_STUDENTS; i++)
    //     pthread_join(studentThreads[i], NULL);
    sem_destroy(&students);
    sem_destroy(&ta);
    pthread_mutex_destroy(&mutex);
    return 0;
}