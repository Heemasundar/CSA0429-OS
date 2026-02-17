#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0; // Indices for producer and consumer

sem_t empty, full;  // Semaphores
pthread_mutex_t mutex; // Mutex lock

void *producer(void *arg) {
    int item;
    for (int i = 1; i <= 10; i++) {
        item = rand() % 100; // Producing a random item

        sem_wait(&empty);  // Wait if buffer is full
        pthread_mutex_lock(&mutex); // Lock buffer

        buffer[in] = item;
        printf("Producer produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock buffer
        sem_post(&full); // Increment full count

        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    for (int i = 1; i <= 10; i++) {
        sem_wait(&full);  // Wait if buffer is empty
        pthread_mutex_lock(&mutex); // Lock buffer

        item = buffer[out];
        printf("Consumer consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock buffer
        sem_post(&empty); // Increment empty count

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prodThread, consThread;

    sem_init(&empty, 0, BUFFER_SIZE); // Initialize empty slots
    sem_init(&full, 0, 0); // Initialize full slots
    pthread_mutex_init(&mutex, NULL); // Initialize mutex

    pthread_create(&prodThread, NULL, producer, NULL);
    pthread_create(&consThread, NULL, consumer, NULL);

    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
