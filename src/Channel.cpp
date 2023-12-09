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
