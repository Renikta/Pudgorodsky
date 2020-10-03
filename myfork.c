#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int myfork(int i, int number_of_processes);

int main(int argc, char* argv[]) {
    int number_of_processes = 0;

    if (argc > 1) {
        number_of_processes = atoi(argv[1]);
        printf("number_of_processes = %d\n", number_of_processes);
    }

    for (int i = 0; i < number_of_processes; i++) {
    myfork(i, number_of_processes);
    }

    return 0;
}

int myfork(int i, int number_of_processes) {
    int something = 0;
    int pid;

    pid = fork();
    if (pid == 0) {
        printf("CHILD: pid = %d, parent's pid = %d\n", getpid(), getppid());
        exit(0);
    }
    else {
        if (i == 0) {
            printf("PARENT: pid = %d\n", getpid());
        }
        if ((i + 1) == number_of_processes) {
            exit(0);
        }
    }
        wait(&something);

    return 0;
}
