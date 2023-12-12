#include "Server.h"

#include "Channel.h"
#include "Connection.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

using namespace MyTinyServer;

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  loop->Loop();
  return 0;
}
