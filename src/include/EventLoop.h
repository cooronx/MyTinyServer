#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>

#include "Epoll.h"
#include "Noncopyable.h"
#include "ThreadPool.h"
namespace MyTinyServer {

class EventLoop : Noncopyable {
 public:
  EventLoop() { epoll_ = new Epoll(); }
  ~EventLoop() { delete epoll_; }

  //* 事件循环
  void Loop();

  void UpdateChannel(Channel *);

  void AddToWorkerThread(std::function<void()> ck) {
    ThreadPool::GetInstance().addTask(std::move(ck));
  }

 private:
  Epoll *epoll_{};
  bool is_quit_{};
};

}  // namespace MyTinyServer

#endif  // EVENTLOOP_H;