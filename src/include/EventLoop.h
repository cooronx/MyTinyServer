#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Epoll.h"
#include "Noncopyable.h"
namespace MyTinyServer {

class EventLoop : Noncopyable {
 public:
  EventLoop() { epoll_ = new Epoll(); }
  ~EventLoop() { delete epoll_; }

  //* 事件循环
  void Loop();

  void UpdateChannel(Channel *);

 private:
  Epoll *epoll_{};
  bool is_quit_{};
};

}  // namespace MyTinyServer

#endif  // EVENTLOOP_H;