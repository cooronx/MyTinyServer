#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

namespace MyTinyServer {

class Server {
 public:
  Server(EventLoop *loop);

  void NewConnection();
  void HandleReadEvent(int);

 private:
  EventLoop *eventloop_;
  Socket *server_socket_;
  InetAddress *server_addr;
  Channel *server_channel;
};

}  // namespace MyTinyServer

#endif