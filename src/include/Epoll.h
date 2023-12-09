#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#include <cstdint>
#include <vector>

#include "Channel.h"
#include "Noncopyable.h"

using std::vector;

namespace MyTinyServer {

/*
 * 有参构造和无参构造没有本质区别，因为size是由内核动态分配的，size这个参数已经被废弃了
 */

class Epoll : Noncopyable {
 public:
  Epoll(int);
  Epoll();
  ~Epoll();

  //* 添加fd到epoll中
  void AddFdToEpoll(int, uint32_t);

  //* 轮询epoll
  [[nodiscard]] vector<Channel *> Poll(int);

  //* 添加channel到epoll中
  //! channel和fd的区别在于，channel是一个被包装过的fd，承载有更多的信息，推荐使用
  void UpdateChannel(Channel *);

 private:
  int epollfd_;
  // 由内核给我们提供给我们的事件表
  epoll_event *events_;
};

}  // namespace MyTinyServer

#endif