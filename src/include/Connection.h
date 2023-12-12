#ifndef CONNECTION_H
#define CONNECTION_H

#include <utility>

#include "Channel.h"
#include "Noncopyable.h"
#include "Socket.h"

namespace MyTinyServer {

class Connection : Noncopyable {
 public:
  Connection(int, EventLoop *);

  void set_delete_conn_callback(function<void(int)> ck) {
    deleteConn_ = std::move(ck);
  }

  ~Connection() {
    delete socket_;
    delete channel_;
  }

 private:
  Socket *socket_;
  Channel *channel_;
  function<void(int)> deleteConn_;
  void Echo(int);
};

}  // namespace MyTinyServer

#endif  // CONNECTION_H