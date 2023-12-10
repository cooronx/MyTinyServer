#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <utility>

#include "Channel.h"
#include "Socket.h"
namespace MyTinyServer {

using std::function;

class Acceptor {
 public:
  Acceptor(EventLoop *);

  void set_connection_callback(function<void(Socket *)> cb) {
    ConnectionCallBack_ = std::move(cb);
  }

  ~Acceptor() {
    delete server_channel;
    delete server_socket;
    delete server_addr;
  }

 private:
  InetAddress *server_addr;
  Socket *server_socket;
  Channel *server_channel;
  EventLoop *loop_;
  function<void(Socket *)> ConnectionCallBack_;

  void AcceptConnection() { ConnectionCallBack_(server_socket); }
};

}  // namespace MyTinyServer

#endif