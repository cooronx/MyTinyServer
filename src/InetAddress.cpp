#include "include/InetAddress.h"

#include <netinet/in.h>

#include <cstdint>
#include <cstring>
#include <iterator>

using namespace MyTinyServer;

InetAddress::InetAddress() : len_(sizeof(addr_)) {
  memset(&addr_, 0, sizeof(addr_));
}

InetAddress::InetAddress(const string &ip, uint16_t port, DomainType domain) {
  memset(&addr_, 0, sizeof(addr_));

  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
  addr_.sin_port = htons(port);

  len_ = sizeof(addr_);
}