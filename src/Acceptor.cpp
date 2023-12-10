#include "include/Acceptor.h"

#include <functional>

#include "util.h"

using namespace MyTinyServer;

Acceptor::Acceptor(EventLoop *loop) : loop_(loop) {
  server_addr = new InetAddress("127.0.0.1", 8888, DomainType::IPV4);
  server_socket = new Socket(DomainType::IPV4, ConnectionType::TCP_CONNECTION);
  server_socket->bind(*server_addr);
  server_socket->listen(SOMAXCONN);

  server_channel = new Channel(server_socket->fd(), loop_);
  function<void()> cb = [this] { AcceptConnection(); };
  server_channel->set_callback(cb);
  server_channel->EnableRead(IOType::BLOCK);
}