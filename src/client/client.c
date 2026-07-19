#include "../common/syscalls.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  int fd = Socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
  Connect(fd, (const struct sockaddr *)&addr, sizeof(addr));

  char msg[] = "hello";
  Write(fd, msg, strlen(msg));

  char rbuf[64];
  Read(fd, rbuf, sizeof(rbuf) - 1);
  printf("server says: %s\n", rbuf);

  Close(fd);

  return EXIT_SUCCESS;
}
