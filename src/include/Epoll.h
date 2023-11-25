#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#include <cstdint>
#include <vector>

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

  // * 添加fd到epoll中
  void add_to_epoll(int, uint32_t);

  // * 轮询epoll
  vector<epoll_event> poll(int);

 private:
  int epollfd_;
// 由内核给我们提供给我们的事件表
  epoll_event *events_;
};

}  // namespace MyTinyServer

#endif