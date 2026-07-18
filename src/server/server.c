#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void do_something(int connfd) {
  char rbuf[64];
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if (n < 0) {
    fprintf(stderr, "Error: read()\n");
    return;
  }
  printf("client says: %s\n", rbuf);

  char wbuf[] = "world";
  write(connfd, wbuf, strlen(wbuf));
}

int main(void) {
  // Initialize socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    fprintf(stderr, "Error: socket()\n");
  }

  // https://stackoverflow.com/questions/3229860/what-is-the-meaning-of-so-reuseaddr-setsockopt-option-linux/3233022#3233022
  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  // Bind socket to 0.0.0.0:1234
  int port_no = 1234;
  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_port = htons(port_no),
                             .sin_addr.s_addr = htonl(0)};
  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
    fprintf(stderr, "Error: bind()\n");
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  rv = listen(fd, SOMAXCONN);
  if (rv) {
    fprintf(stderr, "Error: listen()\n");
    exit(EXIT_FAILURE);
  }

  printf("Listening on port %d\n", port_no);
  while (1) {
    // accept
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
    if (connfd < 0) {
      continue; // error
    }

    do_something(connfd);
    close(connfd);
  }

  return EXIT_SUCCESS;
}
