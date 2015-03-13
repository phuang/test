#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include <vector>

#define HANDLE_EINTR(x) ({ \
  decltype(x) eintr_wrapper_result; \
  do { \
    eintr_wrapper_result = (x); \
  } while (eintr_wrapper_result == -1 && errno == EINTR); \
  eintr_wrapper_result; \
})

bool SendMsg(int fd,
             const std::vector<std::pair<void*, size_t> >& buffers,
             const std::vector<int>& fds) {
  struct msghdr msg = {};
  std::vector<struct iovec> iov(buffers.size());
  int i = 0;
  size_t length = 0;
  for (auto&& buf : buffers) {
    iov[i].iov_base = buf.first;
    iov[i].iov_len = buf.second;
    length += buf.second;
    ++i;
  }
  msg.msg_iov = &iov[0];
  msg.msg_iovlen = iov.size();

  char* control_buffer = NULL;
  if (fds.size()) {
    const size_t control_len = CMSG_SPACE(sizeof(int) * fds.size());
    control_buffer = new char[control_len];
    struct cmsghdr *cmsg;
    msg.msg_control = control_buffer;
    msg.msg_controllen = control_len;
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int) * fds.size());
    memcpy(CMSG_DATA(cmsg), &fds[0], sizeof(int) * fds.size());
    msg.msg_controllen = cmsg->cmsg_len;
  }

  const int flags = MSG_NOSIGNAL;
  const ssize_t r = HANDLE_EINTR(sendmsg(fd, &msg, flags));
  const bool ret = static_cast<ssize_t>(length) == r;
  delete[] control_buffer;
  return ret;
}

static const int kMaxFileDescriptors = 16;

ssize_t RecvMsg(int fd,
                void *buffer,
                size_t length,
                std::vector<int>* fds,
                pid_t *pid) {
  fds->clear();
  *pid = -1;

  struct msghdr msg = {};
  struct iovec iov = { buffer, length };
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  const size_t kControlBuifferSize =
      CMSG_SPACE(sizeof(int) * kMaxFileDescriptors) +
      CMSG_SPACE(sizeof(struct ucred));

  char control_buffer[kControlBuifferSize];
  msg.msg_control = control_buffer;
  msg.msg_controllen = kControlBuifferSize;

  const ssize_t r = HANDLE_EINTR(recvmsg(fd, &msg, 0));
  if (r == -1)
    return -1;

  int* wire_fds = NULL;
  unsigned wire_fds_len = 0;

  if (msg.msg_controllen > 0) {
    struct cmsghdr* cmsg;
    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
      const unsigned payload_len = cmsg->cmsg_len - CMSG_LEN(0);
      if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        wire_fds = reinterpret_cast<int*>(CMSG_DATA(cmsg));
        wire_fds_len = payload_len / sizeof(int);
      } else if (cmsg->cmsg_level == SOL_SOCKET &&
                 cmsg->cmsg_type == SCM_CREDENTIALS) {
        *pid = reinterpret_cast<struct ucred*>(CMSG_DATA(cmsg))->pid;
      }
    }
  }

  if (msg.msg_flags & MSG_TRUNC || msg.msg_flags & MSG_CTRUNC) {
    for (unsigned int i = 0; i < wire_fds_len; ++i)
      close(wire_fds[i]);
    errno = EMSGSIZE;
    return -1;
  }

  if (wire_fds) {
    for (unsigned int i = 0; i < wire_fds_len; ++i)
      fds->push_back(wire_fds[i]);
  }

  return r;
}

static void child_process(int fd) {
  char buf1[] = "Hello World!";
  char buf2[] = "Hello";
  const std::vector<std::pair<void*, size_t> > buffers = {
    { buf1, sizeof(buf1) - 1 },
    { buf2, sizeof(buf2) - 1 },
  };

  const std::vector<int> fds = {
    1, 2, 2, 1
  };
  bool ret;
  ret = SendMsg(fd, buffers, fds);
  fprintf(stderr, "SendfMsg() =%d\n", ret);
  ret = SendMsg(fd, buffers, fds);
  fprintf(stderr, "SendfMsg() =%d\n", ret);

  sleep(2);
}


static void parent_process(int fd) {
  sleep(1);
  char buf[] = "XXX";
  std::vector<int> fds;
  pid_t pid;
  ssize_t ret;
  ret = RecvMsg(fd, buf, sizeof(buf), &fds, &pid);
  fprintf(stderr, "RECV buf=%s fds.size()=%ld pid=%d\n", buf, fds.size(), pid);
  
  ret = RecvMsg(fd, buf, sizeof(buf), &fds, &pid);
  fprintf(stderr, "RECV buf=%s fds.size()=%ld pid=%d\n", buf, fds.size(), pid);
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

