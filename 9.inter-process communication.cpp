#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);

    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *str = (char *)shmat(shmid, NULL, 0);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) { 
        printf("Parent (Writer): Enter a message: ");
        fgets(str, SHM_SIZE, stdin);
        wait(NULL); 
        shmdt(str);
        shmctl(shmid, IPC_RMID, NULL); 
    } else { 
        sleep(1); 
        printf("Child (Reader): Message from shared memory: %s\n", str);
        shmdt(str);
    }

    return 0;
}
