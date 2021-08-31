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
  fprintf(stderr, "Listening on port %d ...", port);

  fd_set master_set, working_set;
  int max_sd = fd;
  FD_ZERO(&master_set);
  FD_SET(fd, &master_set);

  char buffer[BUFSIZ];
  int new_sd;

  while (true) {
    memcpy(&working_set, &master_set, sizeof(master_set));
    int rc = select(max_sd + 1, &working_set, NULL, NULL, NULL);
    if (rc <= 0) {
      perror(" select() failed");
      break;
    }

    int desc_ready = rc;
    for (int i = 0; i <= max_sd && desc_ready > 0; ++i) {
      if (FD_ISSET(i, &working_set)) { // if i is in the working set
        desc_ready--;
      }
      if (i == fd) {
        new_sd = 0;
        while (new_sd != -1) {
          new_sd = accept(fd, NULL, NULL);
          if (new_sd < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) perror("accept");
            break;
          }
          if (new_sd >= FD_SETSIZE)
            break;
          FD_SET(new_sd, &master_set);
          fcntl(new_sd, F_SETFL, O_NONBLOCK);
          if (new_sd > max_sd) max_sd = new_sd;
          fprintf(stderr, "Server: Accept a new connection %d\n", new_sd);
        }
      } else {
        do {
          memset(buffer, 0, sizeof(buffer));
          rc = recv(i, buffer, sizeof(buffer), 0);
          if (rc <= 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
              perror("recv");
              close(i);
              FD_CLR(i, &master_set);
              if (i == max_sd) {
                while (!FD_ISSET(max_sd, &master_set))
                  max_sd--;
              }
              break;
            }
          } else {
            fprintf(stderr, "Server: Receives message from %d of %d size: %s", i, rc, buffer);
          }
        } while (rc > 0);
      }
    }
  }

  for (int i = 0; i <= max_sd; ++i) {
    if (FD_ISSET(i, &master_set))
      close(i);
  }
}
