#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

static inline int Socket(int __domain, int __type, int __protocol) {
  int fd = socket(__domain, __type, __protocol);
  if (fd < 0) {
    fprintf(stderr, "Error: socket()\n");
    exit(EXIT_FAILURE);
  }
  return fd;
}
static inline int Setsockopt(int __fd, int __level, int __optname,
                             const void *__optval, socklen_t __optlen) {
  int rv = setsockopt(__fd, __level, __optname, __optval, __optlen);
  if (rv < 0) {
    fprintf(stderr, "Error: setsockopt()\n");
  }
  return rv;
}

static inline int Bind(int __fd, const struct sockaddr *__addr,
                       socklen_t __len) {
  int rv = bind(__fd, __addr, __len);
  if (rv) {
    fprintf(stderr, "Error: bind()\n");
    exit(EXIT_FAILURE);
  }
  return rv;
}

static inline int Listen(int __fd, int __n) {
  int rv = listen(__fd, __n);
  if (rv) {
    fprintf(stderr, "Error: listen()\n");
    exit(EXIT_FAILURE);
  }
  return rv;
}

static inline int Connect(int __fd, const struct sockaddr *__addr,
                          socklen_t __len) {
  int rv = connect(__fd, __addr, __len);
  if (rv) {
    fprintf(stderr, "Error: connect()\n");
    exit(EXIT_FAILURE);
  }
  return rv;
}

static inline int Accept(int __fd, struct sockaddr *__addr,
                         socklen_t *__restrict __addr_len) {
  int connfd = accept(__fd, __addr, __addr_len);
  if (connfd < 0) {
    fprintf(stderr, "Error: accept()\n");
  }
  return connfd;
}

static inline ssize_t Read(int __fd, void *__buf, size_t __nbytes) {
  int n = read(__fd, __buf, __nbytes);
  if (n < 0) {
    fprintf(stderr, "Error: read()\n");
    exit(EXIT_FAILURE);
  }
  return n;
}

static inline ssize_t Write(int __fd, const void *__buf, size_t __n) {
  int n = write(__fd, __buf, __n);
  if (n < 0) {
    fprintf(stderr, "Error: write()\n");
    exit(EXIT_FAILURE);
  }
  return n;
}

static inline int Close(int __fd) {
  int rv = close(__fd);
  if (rv < 0) {
    fprintf(stderr, "Error: close()\n");
  }
  return rv;
}

#endif
