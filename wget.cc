#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstdio>

#include <netdb.h>      // for gethostbyname
#include <sys/socket.h> // for socket, AF_INET
#include <sys/types.h>  // for SOCK_STREAM
#include <unistd.h>     // for close

using namespace std;

const int kClientSocketError = -1;
int createClientSocket(const string &host, unsigned short port)
{
  struct hostent *he = gethostbyname(host.c_str());
  if (he == NULL)
    return kClientSocketError;

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0)
    return kClientSocketError;

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr = *((struct in_addr *) he->h_addr);

  if (connect(s, (struct sockaddr *) &address, sizeof(address)) == 0)
    return s;

  close(s);
  return kClientSocketError;
}

void setnonblocking(int sock)
{
  int opts;

  opts = fcntl(sock,F_GETFL);
  if (opts < 0) {
    perror("fcntl(F_GETFL)");
    exit(EXIT_FAILURE);
  }
  opts = (opts | O_NONBLOCK);
  if (fcntl(sock,F_SETFL,opts) < 0) {
    perror("fcntl(F_SETFL)");
    exit(EXIT_FAILURE);
  }
  return;
}

bool startsWith(const string& url, const string& prefix) {
  return strncmp(url.c_str(), prefix.c_str(), prefix.size()) == 0;
}

static const string kProtocolPrefix = "http://";
static const string kDefaultPath = "/";
static pair<string, string> parseURL(string url) {
  if (startsWith(url, kProtocolPrefix))
    url = url.substr(kProtocolPrefix.size());
  size_t found = url.find('/');
  if (found == string::npos)
    return make_pair(url, kDefaultPath);
  string host = url.substr(0, found);
  string path = url.substr(found);
  return make_pair(host, path);
}

static void issueRequest(int client, const string& host, const string& path) {
  // FILE* f = fdopen(client, "w");
  ostringstream oss;
  oss << "GET " << path << " HTTP/1.1\r\n";
  oss << "Host: " << host << "\r\n";
  oss << "\r\n";

  // fwrite(oss.str().c_str(), sizeof(char), oss.tellp(), f);
  write(client, oss.str().c_str(), oss.tellp());
}

static void skipHeader(stringstream& ss) {
  string line;
  do {
    getline(ss, line);
    cout << "skipping line: " << line << endl;
  } while (!line.empty() && line != "\r");
}

static string getFileName(const string& path) {
  if (path.empty() || path[path.size() - 1] == '/') return "index.html";
  size_t found = path.rfind('/');
  return path.substr(found + 1);
}

static const size_t kBufferSize = 1024;
static void savePayload(stringstream& ss, const string& filename) {
  ofstream output(filename, ios::binary); // don't assume it's text
  size_t totalBytes = 0;
  while (!ss.fail()) {
    char buffer[kBufferSize] = {'\0'};
    ss.read(buffer, sizeof(buffer));
    totalBytes += ss.gcount();
    output.write(buffer, ss.gcount());
  }
  cout << "Total number of bytes fetched: " << totalBytes << endl;
}

static void handleResponse(int client) {
  char buffer[64];
  int rc;
  int bytesRead = 0;

  int len = 0;
  ioctl(client, FIONREAD, &len);
  cout << "ready for reading! len: " << len << endl;

  do {
    memset(buffer, 0, 64);
    rc = read(client, buffer, sizeof(buffer) - 1);
    if (rc < 0) {
      cout << "rc < 0" << endl;
      if (errno != EWOULDBLOCK && errno != EAGAIN) {
        cout << strerror(errno) << endl;
        close(client);
        break;
      }
    } else if (rc == 0) {
      cout << "RC IS ZERO" << endl;
    } else {
      bytesRead += rc;
      buffer[rc] = '\0';
      // cout << "rc: " << rc << ", buffer: " << buffer << endl;
      cout << buffer << endl;
    }
  } while (rc > 0);
  cout << "REACHED END" << endl;
  cout << "bytesRead: " << bytesRead << endl;

  // skipHeader(ss);
  // savePayload(ss, getFileName(components.second));
  // cout << buffer << endl;
}

static const unsigned short kDefaultHTTPPort = 80;
static void pullContent(const pair<string, string>& components) {
  int client = createClientSocket(components.first, kDefaultHTTPPort);
  if (client == kClientSocketError) {
    cerr << "Could not connect to host named \"" << components.first << "\"." << endl;
    return;
  }

  setnonblocking(client);
  issueRequest(client, components.first, components.second);

  fd_set read_set;
  struct timeval timeout;

  timeout.tv_sec = 60;
  timeout.tv_usec = 0;
  FD_ZERO(&read_set);
  FD_SET(client, &read_set);

  int r = select(client+1, &read_set, NULL, NULL, &timeout);
  if (r < 0) {
    cout << "handle error" << endl;
  }
  if (r == 0) {
    cout << "handle timedout" << endl;
  }
  if (r > 0) {
    handleResponse(client);
  }
}

int main(int argc, char *argv[1]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <url>" << endl;
    return 1;
  }

  pair<string, string> components = parseURL(argv[1]);
  pullContent(components);
  return 0;
}
