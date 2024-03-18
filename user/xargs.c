#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define READ_SUCCESS 1
#define READ_FAIL 0

int readline(char *pos) {
    char* now = pos;
    while (read(0, now, sizeof(char))) {
        if (*now == '\n' || *now == 0 || *now == ' ' || *now == '\t') {
            *now = 0;
            if (now == pos) return READ_FAIL;
            return READ_SUCCESS;
        }
        ++now;
    }
    *now = 0;
    if (now == pos) return READ_FAIL;
    return READ_SUCCESS;
}

int
main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(2, "Usage: [command] | xargs command\n");
        exit(1);
    }

    char *cmd = argv[1];
    char *args[MAXARG];
    args[0] = cmd;
    for (int i = 2; i < argc; ++i) {
        args[i - 1] = argv[i];
    }
    args[argc - 1] = (char*)malloc(sizeof(char) * 512);
    args[argc] = 0;
    while (readline(args[argc - 1])) {
        int pid = fork();
        if (pid == 0) {
            exec(cmd, args);
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}
