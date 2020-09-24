#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>

#define SIZE 100

void myid();

int main() {
    myid();
    return 0;
}

void myid() {
    //Распечатка UID
    uid_t uid = getuid();
    struct passwd* insides = getpwuid(uid);
    printf("uid = %d(%s) ", uid, insides->pw_name);

    //Распечатка GID
    gid_t gid = getgid();
    insides = getpwuid(gid);
    printf("gid = %d(%s) ", gid, insides->pw_name);

    //Распечатка всех групп
    gid_t list[SIZE];

    errno = 0;
    int number_groups = getgroups(SIZE, list);
    if (number_groups < 0)
    {
        perror("[myid] getgroups error\n");
        return;
    }

    printf("groups = ");
    for (int i = 0; i < number_groups; i++) {
        printf("%d\n", list[i]);
        errno = 0;
        insides = getpwuid(list[i]);
        if (insides == NULL)
        {
            perror("[myid] getpwuid error\n");
            return;
        }

        if (i == number_groups - 1) {
            printf("%d(%s) \n", list[i], insides->pw_name);
            break;
        }
        printf("%d(%s), ", list[i], insides->pw_name);
    }
}
# Pudgorodsky
