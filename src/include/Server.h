#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

namespace MyTinyServer {

class Server {
 public:
  Server(EventLoop *loop);

  void NewConnection(Socket *);
  void HandleReadEvent(int);

 private:
  EventLoop *eventloop_;
  Acceptor *acceptor_;
};

}  // namespace MyTinyServer

#endif