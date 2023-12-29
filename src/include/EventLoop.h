#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <exception>
#include <functional>

#include "Epoll.h"
#include "Noncopyable.h"
#include "ThreadPool.h"
namespace MyTinyServer {

class EventLoop : Noncopyable {
 public:
  EventLoop() {
    epoll_ = new Epoll();
    thread_pool_ = new ThreadPool();
  }
  ~EventLoop() { delete epoll_; }

  //* 事件循环
  void Loop();

  void UpdateChannel(Channel *);

  void AddToWorkerThread(std::function<void()> ck) {
    try {
      // std::cout << "this is in loop" << std::endl;
      thread_pool_->addTask(std::move(ck));
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
  }

 private:
  ThreadPool *thread_pool_{};
  Epoll *epoll_{};
  bool is_quit_{};
};

}  // namespace MyTinyServer

#endif  // EVENTLOOP_H;