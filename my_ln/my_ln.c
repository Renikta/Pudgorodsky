#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc == 1 || argc > 5) {
        printf("Bad num args\n");
        exit(EXIT_FAILURE);
    }

    if (!(strcmp(argv[1], "-s")) {
        make_symb();
        return 0;
    }

    make_hard();
    return 0;
}

int make_hard(const char* src, const char* hard) {
    if (src == NULL || hard == NULL) {
        printf("[make_hard] Bad num of args\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    int ret = link(src, hard);
    if (ret < 0) {
        perror("[make_hard] Make hard link error\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
