#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int make_hard(const char* src, const char* hard);
int make_symb(const char* src, const char* symb);

int main(int argc, char* argv[]) {
    if (argc == 1 || argc > 4) {
        printf("Bad num args\n");
        exit(EXIT_FAILURE);
    }

    if (!(strcmp(argv[1], "-s")))  {
        make_symb(argv[2], argv[3]);
        return 0;
    }

    make_hard(argv[1], argv[2]);
    return 0;
}

int make_hard(const char* src, const char* hard) {
    if (src == NULL || hard == NULL) {
        printf("[make_hard] Bad args\n");
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


int make_symb(const char* src, const char* symb) {
    if (src == NULL || symb == NULL) {
        printf("[make_symb] Bad args\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    int ret = symlink(src, symb);
    if (ret < 0) {
        perror("[make_symb] Make symlink error\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
