#include "include/Acceptor.h"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include <functional>

#include "Util.h"

using namespace MyTinyServer;

Acceptor::Acceptor(EventLoop *loop) : loop_(loop) {
  server_addr = new InetAddress("127.0.0.1", 1234, DomainType::IPV4);
  server_socket = new Socket(DomainType::IPV4, ConnectionType::TCP_CONNECTION);
  server_socket->bind(*server_addr);
  server_socket->listen(SOMAXCONN);

  server_channel = new Channel(server_socket->fd(), loop_);
  function<void()> cb = [this] { AcceptConnection(); };
  server_channel->use_thread_pool(false);
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
  auto logger_ = spdlog::get("mainlogger");
  logger_->info("新连接建立 分配的文件描述符fd = {}, 客户端地址: {}",
                new_client_socket->fd(), client_addr.readable_ipv4_address());
}
