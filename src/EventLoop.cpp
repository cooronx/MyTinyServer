#include "include/EventLoop.h"

using namespace MyTinyServer;

void EventLoop::Loop() {
  while (!is_quit_) {
    auto events = epoll_->Poll(-1);
    for (auto event : events) {
      event->HandleEvent();
    }
  }
}

void EventLoop::UpdateChannel(Channel *channel) {
  epoll_->UpdateChannel(channel);
}