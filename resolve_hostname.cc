#include <iostream>
#include <cassert>
#include <netdb.h> // for gethostbyname
#include <sys/socket.h> // for inet_ntop
#include <netinet/in.h> // also for inet_ntoa
#include <arpa/inet.h> // also for inet_ntoa
using namespace std;

static void publishIPAddressInfo(const string& host) {
  struct hostent *he = gethostbyname(host.c_str());
  if (he == NULL) {
    cout << host << " could not be resolved to an address." << endl;
    return;
  }

  assert(he->h_addrtype == AF_INET);
  assert(he->h_length == sizeof(struct in_addr));

  cout << "Official name is \"" << he->h_name << "\"" << endl;
  cout << "IP Addresses: " << endl;
  struct in_addr **addressList = (struct in_addr **) he->h_addr_list;
  while (*addressList != NULL) {
    char str[INET_ADDRSTRLEN];
    cout << "+ " << inet_ntop(AF_INET, *addressList, str, INET_ADDRSTRLEN) << endl;
    addressList++;
  }
}

static void publishIPv6AddressInfo(const string& host) {
  struct hostent *he = gethostbyname2(host.c_str(), AF_INET6);
  if (he == NULL) {
    cout << host << " could not be resolved to an address." << endl;
    return;
  }

  assert(he->h_addrtype == AF_INET6);
  assert(he->h_length == sizeof(struct in6_addr));

  cout << "Official name is \"" << he->h_name << "\"" << endl;
  cout << "IPv6 Addresses: " << endl;
  struct in6_addr **addressList = (struct in6_addr **) he->h_addr_list;
  while (*addressList != NULL) {
    char str[INET6_ADDRSTRLEN];
    cout << "+ " << inet_ntop(AF_INET6, *addressList, str, INET6_ADDRSTRLEN) << endl;
    addressList++;
  }
}

static void resolveHostnames() {
  while (true) {
    cout << "Enter a host name: ";
    string host;
    getline(cin, host);
    if (host.empty()) break;
    publishIPAddressInfo(host);
  }
  cout << "Goodbye" << endl;
}


int main(int argc, char *argv[]) {
  resolveHostnames();
  return 0;
}
