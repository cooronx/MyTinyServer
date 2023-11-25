#include "include/Socket.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>

using namespace MyTinyServer;
// domain - IPV4 or IPV6
// type   - STREAM : tcp ,DGRAM : udp
// protocol - usually 0
Socket::Socket(DomainType domain, ConnectionType conn)
    : domain_(domain), conn_(conn) {
  int dm = (domain == DomainType::IPV4 ? PF_INET : PF_INET6);
  int cn = (conn == ConnectionType::TCP_CONNECTION ? SOCK_STREAM : SOCK_DGRAM);

  socket_ = ::socket(dm, cn, 0);

  Error::error_check(socket_ == -1, "socket: create socket failed!\n");
}

Socket::Socket(int fd) : socket_(fd) {
  Error::error_check(socket_ == -1, "socket: copy socket failed!\n");
}

void Socket::bind(const InetAddress &addr) {
  Error::error_check(::bind(socket_, (sockaddr *)&addr.addr_, addr.len_) == -1,
                     "socket :bind failed!\n");
}

void Socket::listen(int cnt) {
  Error::error_check(::listen(socket_, cnt), "socket: listen failed!\n");
}

void Socket::setnoblock() {
  int old_option = ::fcntl(socket_, F_GETFL);
  ::fcntl(socket_, F_SETFL, old_option | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr) {
  int client_sockfd =
      ::accept(socket_, (struct sockaddr *)&addr->addr_, &addr->len_);

  Error::error_check(client_sockfd == -1, "socket: accept failed!\n");

  return client_sockfd;
}

Socket::~Socket() {
  // close 并不代表关闭了这个连接，只是把socket_的引用计数-1
  close(socket_);
}