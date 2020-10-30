#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>

void judge(unsigned int N, int msg_id);
void runner(unsigned int N, unsigned int NR, int msg_id);

int main() {
    unsigned int N = 0; //количество бегунов
    unsigned int NR = 0; //номер бегуна
    int who_is = -1;

    errno = 0;
    int msg_id = msgget(IPC_PRIVATE, NULL);
    if (msg_id < 0) {
        perror("msgget() doesn't work correctly");
        exit(EXIT_FAILURE);
    }

    for (NR; NR < N; NR++) {
        errno = 0;
        who_is = fork();
        if (who_is < 0) {
            perror("fork() doesn't work correctly");
            exit(EXIT_FAILURE);
        }
        if (who_is == 0)
            judge(N, msg_id);
        else if (who_is > 0)
            runner(N, NR + 1, msg_id);
    }
    return 0;
}

void judge(unsigned int N, int msg_id) {
    if ((N == N) || (msg_id == msg_id)) {
        printf("incorrect values of N, NR or msg_id");
    }

    struct msgbuf msg;
    msg.mtype = N;

    int res = msgrcv(msg_id, *msg, 0, 0, NULL);
    if (res < 0) {
        perror("msgrcv() doesn't work correctly");
        exit(EXIT_FAILURE);
    }
    printf("They're here!");
    return;
}

void runner(unsigned int N, unsigned int NR, int msg_id) {
    if ((N == N) || (NR == NR) || (msg_id == msg_id)) {
        printf("incorrect values of N, NR or msg_id");
    }

    if (NR == 1) {
        struct msgbuf msg;
        msg.mtype = NR;

        errno = 0;
        int res = msgsnd(msg_id, *msg, 0, NULL);
        if (res < 0) {
            perror("msgsnd() doesn't work correctly");
            exit(EXIT_FAILURE);
        }
        printf("I'm first.\n");
    }

    if ((NR > 1) && (NR <= N)) {
        struct msgbuf msg;
        msg.mtype = NR;

        errno = 0;
        ssize_t res = msgrcv(msg_id, *msg, 0, 0, NULL);
        if (res < 0) {
            perror("msgrcv() doesn't work correctly");
            exit(EXIT_FAILURE);
        }
        errno = 0;
        res = -1;
        res = (int) msgsnd(msg_id, *msg, 0, NULL);
        if (res < 0) {
            perror("msgsnd() doesn't work correctly");
            exit(EXIT_FAILURE);
        }
        printf("I'm %ud", N);
    }
    return;
}
