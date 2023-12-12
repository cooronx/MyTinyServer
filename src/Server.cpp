#include "include/Server.h"

#include <functional>

#include "Socket.h"
#include "util.h"

using namespace MyTinyServer;
using std::function;

Server::Server(EventLoop *loop) : eventloop_(loop) {
  acceptor_ = new Acceptor(loop);
  function<void(Socket *)> cb = [this](auto &&PH1) {
    NewConnection(std::forward<decltype(PH1)>(PH1));
  };
  acceptor_->set_connection_callback(cb);
}

void Server::NewConnection(Socket *client_socket) {
  auto conn = new Connection(client_socket->fd(), eventloop_);
  function<void(int)> cb = [this](int fd) { DeleteConnection(fd); };
  conn->set_delete_conn_callback(cb);
  connections_[client_socket->fd()] = conn;
}

void Server::DeleteConnection(int fd) {
  auto conn = connections_[fd];
  connections_.erase(fd);
  delete conn;
}