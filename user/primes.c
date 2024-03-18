#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void get_prime(int fd) {
    char num;
    if (!read(fd, &num, sizeof(char))) {
        return;
    }
    printf("prime %d\n", num);
    int p[2];
    pipe(p);
    if (fork()) {
        close(p[1]);
        get_prime(p[0]);
        close(p[0]);
    } else {
        close(p[0]);
        char first = num;
        while (read(fd, &num, sizeof(char))) {
            if (num % first) {
                write(p[1], &num, sizeof(char));
            }
        }
        close(fd);
        close(p[1]);
        exit(0);
    }
}

int
main(int argc, char *argv[]) {

    if (argc > 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int p[2];
    pipe(p);
    if (fork()) {
        close(p[1]);
        get_prime(p[0]);
        close(p[0]);
    } else {
        close(p[0]);
        for (char i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(char));
        }
        close(p[1]);
    }
    exit(0);
}
