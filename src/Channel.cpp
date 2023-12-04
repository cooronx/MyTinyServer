#include "Channel.h"

#include "Epoll.h"
#include "util.h"

using namespace MyTinyServer;

Channel::Channel(int fd, Epoll* epoll) : fd_(fd) {
  epoll_ = epoll;
  in_epoll_ = false;
}

void Channel::EnableRead(bool blocking) {
  events_ = EPOLLIN;
  if (!blocking) {
    HelperFunc::setnoblock(fd_);
    events_ |= EPOLLET;
  }
  epoll_->UpdateChannel(this);
}
