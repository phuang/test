#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

static void child_process(int fd) {
  char buf1[] = "Hello World!";
  char buf2[] = "Hello";
  
  struct msghdr msgh;
  struct iovec iov[2];

  iov[0].iov_base = buf1;
  iov[0].iov_len = sizeof(buf1);
  iov[1].iov_base = buf2;
  iov[1].iov_len = sizeof(buf2);

  memset(&msgh, 0, sizeof(msgh));
  msgh.msg_iov = iov;
  msgh.msg_iovlen = 2;
  sendmsg(fd, &msgh, 0);
  sleep(1);
}


static void parent_process(int fd) {
  char buf1[] = "XXXXX World!";
  char buf2[] = "XXXXX";
  
  struct msghdr msgh;
  struct iovec iov[2];

  iov[0].iov_base = buf1;
  iov[0].iov_len = sizeof(buf1);
  iov[1].iov_base = buf2;
  iov[1].iov_len = sizeof(buf2);

  memset(&msgh, 0, sizeof(msgh));
  msgh.msg_iov = iov;
  msgh.msg_iovlen = 2;
  int ret = recvmsg(fd, &msgh, 0);
  fprintf(stderr, "ret=%d buf1=%s buf2=%s\n", ret, buf1, buf2);

  sleep(1);
}

int main() {
  int sv[2];

  socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

  int fd_parent = sv[0];
  int fd_child = sv[1];

  pid_t pid = fork();

  if (pid != 0) {
    close(fd_parent);
    child_process(fd_child);
  } else {
    close(fd_child);
    parent_process(fd_parent);
  }
}

