#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Function that each thread will execute
void* print_message(void* thread_id) {
    int tid = *(int*)thread_id;
    printf("Thread %d: Running...\n", tid);
    sleep(1); // Simulate some work
    printf("Thread %d: Finished.\n", tid);
    return NULL;
}

int main() {
    const int NUM_THREADS = 3;
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Creating threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, print_message, (void*)&thread_ids[i]);
    }

    // Waiting for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads finished execution.\n");
    return 0;
}
