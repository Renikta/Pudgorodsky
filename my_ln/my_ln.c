#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc == 1 || argc > 5)
    {
        printf("Bad num args\n");
        exit(EXIT_FAILURE);
    }

    if (!(strcmp(argv[1], "-s")))
    {
        make_symb();
        return 0;
    }

    make_hard();
    return 0;
}
