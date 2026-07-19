#include "../common/syscalls.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void do_something(int connfd) {
  char rbuf[64];
  Read(connfd, rbuf, sizeof(rbuf) - 1);
  printf("client says: %s\n", rbuf);

  char wbuf[] = "world";
  Write(connfd, wbuf, strlen(wbuf));
}

int main(void) {
  // Initialize socket
  int fd = Socket(AF_INET, SOCK_STREAM, 0);

  // https://stackoverflow.com/questions/3229860/what-is-the-meaning-of-so-reuseaddr-setsockopt-option-linux/3233022#3233022
  int val = 1;
  Setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  // Bind socket to 0.0.0.0:1234
  int port_no = 1234;
  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_port = htons(port_no),
                             .sin_addr.s_addr = htonl(0)};
  Bind(fd, (const struct sockaddr *)&addr, sizeof(addr));

  // Listen for connections
  Listen(fd, SOMAXCONN);
  printf("Listening on port %d\n", port_no);

  // Accept and serve client connections in loop
  while (1) {
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int connfd = Accept(fd, (struct sockaddr *)&client_addr, &addrlen);
    if (connfd < 0) {
      continue; // error
    }

    do_something(connfd);
    Close(connfd);
  }

  return EXIT_SUCCESS;
}
