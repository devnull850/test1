
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

const char FILENAME[] = "blob";

void foo(void);

int main(void) {
    size_t bytes, pagesize;
    unsigned char *buf;
    void *p, *a;
    FILE *fd;

    if ((fd = fopen(FILENAME, "rb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0, SEEK_END);
    bytes = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    if ((buf = malloc(bytes)) == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buf, 1, bytes, fd) != bytes) {
        fprintf(stderr, "error with reading file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    p = a = (void *) foo;
    pagesize = sysconf(_SC_PAGESIZE);
    p -= (size_t) p % pagesize;

    if (mprotect(p, pagesize, PROT_READ|PROT_WRITE|PROT_EXEC) == -1) {
        fprintf(stderr, "error with page permissions\n");
        exit(EXIT_FAILURE);
    }

    memcpy(a, buf, bytes);
    free(buf);

    foo();

    return EXIT_SUCCESS;
}

void foo(void) {
    puts("The quick brown fox jumped over the lazy sleeping dog.");
}

