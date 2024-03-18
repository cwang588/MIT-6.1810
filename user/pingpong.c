#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {

    if (argc > 1) {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    int p[2][2];
    pipe(p[0]);
    pipe(p[1]);
    char tmp[2];
    if (fork() == 0) {
        close(p[0][1]);
        close(p[1][0]);
        read(p[0][0], tmp, 1);
        close(p[0][0]);
        printf("%d: received ping\n", getpid());
        write(p[1][1], " ", 1);
        close(p[1][1]);
    } else {
        close(p[1][1]);
        close(p[0][0]);
        write(p[0][1], " ", 1);
        close(p[0][1]);
        read(p[1][0], tmp, 1);
        close(p[1][0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}
