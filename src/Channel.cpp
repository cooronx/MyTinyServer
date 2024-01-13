#include "Channel.h"

#include "EventLoop.h"
#include "util.h"

using namespace MyTinyServer;

Channel::Channel(int fd, EventLoop* epoll) : fd_(fd) {
  loop_ = epoll;
  in_epoll_ = false;
}

void Channel::EnableRead(IOType type) {
  events_ = EPOLLIN;
  if (type == IOType::NOBLOCK) {
    HelperFunc::setnoblock(fd_);
    events_ |= EPOLLET;
  }
  loop_->UpdateChannel(this);
}

//* 放入工作线程进行使用
void Channel::HandleEvent() { callback_(); }