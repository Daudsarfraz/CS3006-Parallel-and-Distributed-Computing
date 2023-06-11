#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 0x1234
#define SHM_SIZE 1024

int main() {
    int shmid;
    int *p;

    // Allocate shared memory
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory
    p = (int *)shmat(shmid, NULL, 0);
    if (p == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Create a child process
    if (fork() == 0) {
        // Child process
        *p = 42;
        printf("Child process: p=%d\n", *p);
        exit(0);
    } else {
        // Parent process
        sleep(1);
        printf("Parent process: p=%d\n", *p);
    }

    // Detach the shared memory
    if (shmdt(p) < 0) {
        perror("shmdt");
        exit(1);
    }

    // Deallocate the shared memory
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
