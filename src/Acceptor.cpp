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

void Acceptor::AcceptConnection() {
  InetAddress client_addr;
  int new_fd = server_socket->accept(&client_addr);
  auto new_client_socket = new Socket(new_fd);
  //* 新建立的连接设置为非阻塞 ET触发
  HelperFunc::setnoblock(new_client_socket->fd());
  ConnectionCallBack_(new_client_socket);
  std::cout << "new connection fd = " << new_client_socket->fd()
            << " address = " << client_addr.readable_ipv4_address()
            << std::endl;
}
