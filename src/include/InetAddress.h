#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

#include "arpa/inet.h"
#include "util.h"

using std::array;
using std::string;
using std::string_view;

namespace MyTinyServer {

// ! 请记得什么是sizeof sizeof!!!!
class InetAddress {
 public:
  InetAddress();
  explicit InetAddress(const string &, uint16_t port, DomainType);

  // helper

  // *从二进制转换为点分十进制，返回一个字符串
  [[nodiscard]] string readable_ipv4_address() const {
    char real_ip_address[18] = {0};

    inet_ntop(AF_INET, &addr_.sin_addr, real_ip_address,
              sizeof(real_ip_address));

    return real_ip_address;
  }

 public:
  sockaddr_in addr_;
  socklen_t len_;
};

};  // namespace MyTinyServer

#endif