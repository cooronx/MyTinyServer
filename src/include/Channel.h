#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>

namespace MyTinyServer {

class Epoll;

class Channel {
 public:
  Channel() = delete;
  Channel(int, Epoll *);

  // getter
  [[nodiscard]] int fd() const { return this->fd_; }
  [[nodiscard]] uint32_t events() const { return this->events_; }
  [[nodiscard]] uint32_t read_events() const { return this->read_events_; }
  [[nodiscard]] bool in_epoll() const { return this->in_epoll_; }

  void set_in_epoll() { in_epoll_ = true; }
  void set_read_events(uint32_t ev) { read_events_ = ev; }
  //* 开启读
  //* 会自动把阻塞的fd转换为非阻塞的
  void EnableRead(bool);

 private:
  int fd_;
  //! 这个指针不应该被当前类释放
  Epoll *epoll_;

  //* 当前Channel希望监听的事件
  uint32_t events_;

  //* 从Epoll中读取到的事件
  uint32_t read_events_;
  //* 当前这个Channel是否处于Epoll中
  bool in_epoll_;
};

}  // namespace MyTinyServer

#endif