#include "include/Connection.h"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <functional>
#include <iostream>

#include "Buffer.h"
#include "util.h"

using namespace MyTinyServer;
using std::function;

Connection::Connection(int fd, EventLoop *loop) {
  socket_ = new Socket(fd);
  channel_ = new Channel(fd, loop);
  function<void()> ck = [this, fd] { Echo(fd); };

  channel_->set_callback(ck);
  channel_->EnableRead(IOType::NOBLOCK);
}

void Connection::Echo(int sockfd) {
  char buf[1024];
  while (true) {
    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = recv(sockfd, buf, sizeof(buf), 0);
    auto *buffer = new Buffer(buf);
    if (bytes_read > 0) {
      printf("message from client fd %d: %s\n", sockfd, buffer->c_str());
      send(sockfd, buffer->c_str(), buffer->size(), 0);

    } else if (bytes_read == -1 && errno == EINTR) {
      //客户端正常中断、继续读取
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) ||
                (errno ==
                 EWOULDBLOCK))) {  //非阻塞IO，这个条件表示数据全部读取完毕
      printf("finish reading once\n");
      break;
    } else if (bytes_read == 0) {  // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", sockfd);
      // 回调删除fd
      deleteConn_(sockfd);
      auto logger = spdlog::get("mainlogger");
      logger->info("fd = {} 断开连接", sockfd);
      break;
    }
    delete buffer;
  }
}