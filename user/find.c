#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *filename) {
    int fd;
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(1, "find: cannot open %s\n", path);
        close(fd);
        return;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        fprintf(1, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (st.type != T_DIR) {
        close(fd);
        return;
    }
    char *p = path + strlen(path);
    *(p++) = '/';
    struct dirent de;
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;
        if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
            continue;
        }
        strcpy(p, de.name);
        if (!strcmp(de.name, filename)) {
            printf("%s\n", path);
        }
        find(path, filename);
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find directory filename\n");
        exit(1);
    }
    char *path = argv[1], *filename = argv[2];
    char buf[512];
    strcpy(buf, path);
    find(buf, filename);
    exit(0);
}
