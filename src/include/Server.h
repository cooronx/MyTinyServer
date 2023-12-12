#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

#include <map>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

namespace MyTinyServer {

class Server {
 public:
  Server(EventLoop *loop);

 private:
  std::map<int, Connection *> connections_;
  EventLoop *eventloop_;
  Acceptor *acceptor_;

  // 回调函数
  void NewConnection(Socket *);
  void DeleteConnection(int);
};

}  // namespace MyTinyServer

#endif