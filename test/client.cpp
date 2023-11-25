#include <cstring>
#include <iostream>
#include <memory>

#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

using namespace MyTinyServer;

int main() {
  std::unique_ptr<Socket> socket(
      new Socket(DomainType::IPV4, ConnectionType::TCP_CONNECTION));

  InetAddress server_addr("127.0.0.1", 8888, DomainType::IPV4);

  int ret = connect(socket->fd(), (struct sockaddr *)&server_addr.addr_,
                    sizeof(server_addr.addr_));

  Error::error_check(ret == -1, "client :connect failed!\n");

  std::cout << "connect to the server " << server_addr.readable_ipv4_address();

  //输入字符串
  std::string content;
  char buf[1024] = {0};

  while (true) {
    memset(buf, 0, sizeof(buf));

    std::cin >> content;

    ssize_t n_write = send(socket->fd(), content.c_str(), content.size(), 0);

    Error::error_check(n_write < 0, "client: send error!\n");
    ssize_t n_read = recv(socket->fd(), buf, sizeof(buf), 0);

    Error::error_check(n_read < 0, "recv from server error\n");
    if (n_read == 0) {
      std::cout << ("client: server disconnected\n");
      break;
    }
    std::cout << "message from server: " << buf << std::endl;
  }
  return 0;
}
