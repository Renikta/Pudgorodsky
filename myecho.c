#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc > 1) {

        char key_n = 1;
        if (argv[1][0] == '-')
            key_n = 0;
        while (--argc > 0)
            printf("%s%s", *++argv, (argc > 1) ? " " : "");
        if (key_n)
            printf("\n");

    }
    return 0;
}
