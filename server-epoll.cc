#include <cassert>
#include <cctype>
#include <cerrno>
#include <fcntl.h>
#include <sched.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <list>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/epoll.h>

#define MAX_EVENTS 5
#define MAXFDS 16*1024

inline double tstamp(void) {
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    return tv.tv_sec + tv.tv_nsec * 1.0e-9;
}

int open_listen_socket(int port) {
  // Create socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  // Useful options: close-on-exec, reuse-address
  int r = fcntl(fd, F_SETFD, FD_CLOEXEC);
  if (r < 0) {
    perror("fcntl");
    close(fd);
    return -1;
  }

  int yes = 1;
  r = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if (r < 0) {
    perror("setsockopt");
    close(fd);
    return -1;
  }

  // Bind to port
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = INADDR_ANY;
  r = bind(fd, (struct sockaddr*) &address, sizeof(address));
  if (r < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  // Actually start listeing
  r = listen(fd, 100);
  if (r < 0) {
    perror("listen");
    close(fd);
    return -1;
  }

  return fd;
}

inline bool is_valid_key(const char* s) {
    const char* begin = s;
    while (*s && !isspace((unsigned char) *s)) {
        ++s;
    }
    return *s == 0 && s != begin;
}

inline unsigned string_hash(const char* s) {
    return (unsigned char) *s;
}

void raise_file_limit() {
    struct rlimit rlim;
    int r = getrlimit(RLIMIT_NOFILE, &rlim);
    if (r >= 0 && rlim.rlim_cur != RLIM_INFINITY) {
        if (geteuid() == 0 && rlim.rlim_max < 10240
            && rlim.rlim_max != RLIM_INFINITY) {
            rlim.rlim_cur = rlim.rlim_max = 10240;
        } else {
            rlim.rlim_cur = rlim.rlim_max;
        }
        setrlimit(RLIMIT_NOFILE, &rlim);
    }
}

int remove_trailing_whitespace(char* s) {
    int len = strlen(s);
    while (len > 0 && isspace((unsigned char) s[len - 1])) {
        --len;
        s[len] = 0;
    }
    return len;
}

inline int make_nonblocking(int fd) {
    return fcntl(fd, F_SETFL, O_NONBLOCK);
}

int main(int argc, char** argv) {
  int port = 6169;
  if (argc >= 2) {
    port = strtol(argv[1], nullptr, 0);
    assert(port > 0 && port <= 65535);
  }

  // Prepare listening socket
  int fd = open_listen_socket(port);
  fcntl(fd, F_SETFL, O_NONBLOCK);
  assert(fd >= 0);
  fprintf(stderr, "Listening on port %d...\n", port);

  int epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    perror("epoll");
    exit(1);
  }

  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event)) {
    perror("epoll_ctl");
    close(epoll_fd);
    exit(1);
  }

  struct epoll_event* events = (epoll_event *)calloc(MAXFDS, sizeof(struct epoll_event));

  char buffer[BUFSIZ];
  int new_sd, rc;

  while (true) {
    int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for (int i = 0; i < event_count; ++i) {
      if (events[i].data.fd == fd) {
        new_sd = 0;
        while (new_sd != -1) {
          new_sd = accept(fd, NULL, NULL);
          if (new_sd < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) perror("accept");
            break;
          }
          if (new_sd >= MAXFDS)
            break;
          fcntl(new_sd, F_SETFL, O_NONBLOCK);
          struct epoll_event e;
          e.data.fd = new_sd;
          e.events |= EPOLLIN;
          if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_sd, &e) < 0) {
            perror("epoll_ctl");
            break;
          }
          fprintf(stderr, "Server: Accept a new connection %d\n", new_sd);
        }
      } else {
        do {
          memset(buffer, 0, sizeof(buffer));
          rc = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
          if (rc <= 0) {
            if (errno != EWOULDBLOCK) {
              perror("recv");
              close(events[i].data.fd);
              break;
            }
          } else {
            fprintf(stderr, "Server: Receives message from %d of %d size: %s", i, rc, buffer);
          }
        } while (rc > 0);
      }
    }
  }

  close(epoll_fd);
}
