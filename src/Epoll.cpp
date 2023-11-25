#include "include/Epoll.h"

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "util.h"

using namespace MyTinyServer;

Epoll::Epoll(int size) {
  epollfd_ = epoll_create(size);
  Error::error_check(epollfd_ == -1, "Epoll: create failed!\n");

  events_ = new epoll_event[EPOLL_MAX_EVENTS];
  memset(events_, 0, sizeof(*events_) * EPOLL_MAX_EVENTS);
}

Epoll::Epoll() {
  epollfd_ = epoll_create(1);
  Error::error_check(epollfd_ == -1, "Epoll: create failed!\n");

  events_ = new epoll_event[EPOLL_MAX_EVENTS];
  memset(events_, 0, sizeof(*events_) * EPOLL_MAX_EVENTS);
}

void Epoll::add_to_epoll(int fd, uint32_t op) {
  // * 如果是ET（边缘触发），就必须设置为非阻塞
  if (!(fcntl(fd, F_GETFL) & O_NONBLOCK)) {
    if (op & EPOLLET) {
      Error::error_check(true, "Epoll: add failed, fd not nonblock!\n");
    }
  }

  epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = fd;
  event.events = op;
  int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
  Error::error_check(ret == -1, "Epoll: add failed!\n");
}

// ! timeout 设置等待时间，-1代表着一直阻塞直到由事件发生
vector<epoll_event> Epoll::poll(int timeout) {
  int cnt = epoll_wait(epollfd_, events_, EPOLL_MAX_EVENTS, timeout);
  Error::error_check(cnt == -1, "Epoll: poll failed!\n");

  vector<epoll_event> activeEvents;
  activeEvents.reserve(cnt);
  for (int i = 0; i < cnt; ++i) {
    activeEvents.emplace_back(events_[i]);
  }
  return activeEvents;
}

Epoll::~Epoll() {
  close(epollfd_);
  delete[] events_;
}