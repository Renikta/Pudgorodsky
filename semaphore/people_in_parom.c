#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>

#define NUMBER_SEM 3
#define CAN_BRIDGE 0
#define CAN_SHIP 1
#define STATUS 2
#define WAIT_GUEST 1
#define NO_WAIT -1

#define CHECK(function) \
    if ((function) < 0) { \
        perror(#function); \
        exit(EXIT_FAILURE); \
    } \

#define Resize(SEM_NAME, VAL) do {\
    sops.sem_num = SEM_NAME;  \
    sops.sem_op = VAL; \
    sops.sem_flg = 0; \
    CHECK( semop(semid, &sops, 1) ); \
    } while(0); \

void passenger(int semid) {
    struct sembuf sops;
    while(!semctl(semid, STATUS, GETVAL)) {}
        Resize(CAN_SHIP, -1);
        Resize(CAN_BRIDGE, -1);
        Resize(CAN_BRIDGE, 1);
    while(semctl(semid, STATUS, GETVAL)) {}
        Resize(CAN_BRIDGE, -1);
        Resize(CAN_BRIDGE, 1);
        Resize(CAN_SHIP, 1);
    return NULL;
};


void ship(int semid, int max_guest, int capasity, int number_of_circles) {
    struct sembuf sops;
    int num_guest = 0;
    printf("Good afternoon. Let's start our journey.\n");
    for (int i = 0; i < number_of_circles; i++) {
        printf("Wait for visitors...\n");
        if(num_guest != 0) {
            Resize(STATUS, WAIT_GUEST);
        }
        printf("Let's go!\n");
        sleep(2);
        num_guest += capasity - semctl(semid, CAN_SHIP, GETVAL);
        Resize(STATUS, NO_WAIT);
        printf("We're here!\n");
    }
    printf("The journey is over, have a Nice day!\n");
    return NULL;
};

int main(int argc, char* argv[]) {
////////////////////////////////////////////////////////////////////////////////
    //ОБЪЯВЛЕНИЯ
    char print_buf[BUFSIZ];
    setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
    struct sembuf sops;
    pid_t my_pid;
    int guest_max = 10;
    int strenght = 2;
    int capacity = 5;

    if (argc >= 4) {
        guest_max = atoi(argv[1]);
        strenght = atoi(argv[2]);
        capacity = atoi(argv[3]);
    }
////////////////////////////////////////////////////////////////////////////////
    //СЕМАФОРЫ
    errno = 0;
    int semid = semget(IPC_PRIVATE, NUMBER_SEM, 0777);
    if(semid < 0) {
        perror("Error: semget failed\n");
        exit(EXIT_FAILURE);
    }

    Resize(CAN_BRIDGE, strenght);
    Resize(CAN_SHIP, capacity);
    Resize(STATUS, WAIT_GUEST);


////////////////////////////////////////////////////////////////////////////////
    //FORKи

    my_pid = fork();
    if (my_pid < 0) {
        perror("Error: fork() failed");
        exit(EXIT_FAILURE);
    }

    if (my_pid == 0) {
        if (guest_max/capacity < 1) {
            ship(semid, guest_max, guest_max, 1);
            return 0;
        }
        else {
            ship(semid, guest_max, capacity, guest_max/capacity);
            return 0;
        }
    }
    if (my_pid > 0) {
        for (int i = 0; i < guest_max; i++) {
            my_pid = fork();
            if (my_pid < 0) {
                perror("Error: fork() failed");
                exit(EXIT_FAILURE);
            }
            if (my_pid == 0) {
                passenger(semid);
                return 0;
            }
        }
    }
    return 0;
}
