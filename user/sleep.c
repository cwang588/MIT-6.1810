#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int i;

    if (argc < 2) {
        fprintf(2, "Usage: sleep clock ticks...\n");
        exit(1);
    }

    for (i = 1; i < argc; ++i) {
        int t = atoi(argv[i]);
        if (t <= 0) {
            fprintf(2, "Clock ticks should be positive integers.\n");
            exit(1);
        }
        sleep(t);
    }
    exit(0);
}
