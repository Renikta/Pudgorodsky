#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define SIZE_READ 20
#define SIZE_BUFER 100
enum flow {
        STDOUT = 0,
        STDIN = 1,
};

int mypipe();

int main() {

    if (mypipe() == 0)
        return 0;
    return -1;
}

int mypipe() {
    struct timeval start, finish;
    struct timezone tz;
    char const argv[1];
    int pipefd[2];

    pipe(pipefd);

    gettimeofday(&start, &tz);
    int who_is = fork();

    if (who_is == 0) { //я ребенок
        dup2(pipefd[1], STDIN);
        execl("hello", argv);
    }
    else { //я родитель
        char bufer[SIZE_BUFER];
        dup2(pipefd[0], STDOUT);

        gettimeofday(&finish, &tz);
        printf("time: %f\n", finish.tv_sec + 1e-6 * finish.tv_usec - start.tv_sec - 1e-6 * start.tv_usec);

        wait(NULL);
        gettimeofday(&finish, &tz);
        printf("time: %f\n", finish.tv_sec + 1e-6 * finish.tv_usec - start.tv_sec - 1e-6 * start.tv_usec);

        read(STDOUT, (void*) bufer, SIZE_READ);
        for(int i = 0; bufer[i] != '\n'; i++) {
            printf("%c", bufer[i]);
        }
    }
    return 0;
}
