#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>

#include <functional>
#include <utility>

#include "Util.h"

namespace MyTinyServer {

class EventLoop;

using std::function;

//* 其实这个类就是fd的包装类，包含了fd的各种事件
class Channel {
 public:
  Channel() = delete;
  Channel(int, EventLoop *);
  ~Channel() {}

  //* getter
  [[nodiscard]] int fd() const { return this->fd_; }
  [[nodiscard]] uint32_t events() const { return this->events_; }
  [[nodiscard]] uint32_t read_events() const { return this->read_events_; }
  [[nodiscard]] bool in_epoll() const { return this->in_epoll_; }

  //* setter
  void set_in_epoll() { in_epoll_ = true; }
  void set_read_events(uint32_t ev) { read_events_ = ev; }
  void set_callback(function<void()> ck) { callback_ = std::move(ck); }
  void use_thread_pool(bool state) { isUseThread = false; }

  //* 开启对这个Channel的监听，读写，这里会把channel加入到epoll中
  //* 会自动把阻塞的fd转换为非阻塞的
  void EnableRead(IOType);

  void HandleEvent();

 private:
  bool isUseThread = true;
  // *fd由socket来释放
  int fd_;
  //! 这个指针不应该被当前类释放
  EventLoop *loop_;

  //* 当前Channel希望监听的事件
  uint32_t events_;

  //* 从Epoll中读取到的事件
  uint32_t read_events_;

  //* 当前这个Channel是否处于Epoll中
  bool in_epoll_;

  function<void()> callback_;
};

}  // namespace MyTinyServer

#endif