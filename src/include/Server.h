#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

#include <map>
#include <mutex>
#include <vector>

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ThreadPool.h"

namespace MyTinyServer {

class Server {
 public:
  Server(EventLoop *loop);

 private:
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_reactors_;
  ThreadPool *thpool_ = nullptr;
  EventLoop *main_reactor_;  //* 主从Reactor模式
  Acceptor *acceptor_ = nullptr;
  std::mutex mtx_;

 private:
  // 回调函数
  void NewConnection(Socket *);
  void DeleteConnection(int);
  int RandomSubIndex(int);
};

}  // namespace MyTinyServer

#endif