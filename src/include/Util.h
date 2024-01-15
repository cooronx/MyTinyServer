#ifndef UTIL_H
#define UTIL_H

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
namespace MyTinyServer {

//* 常量定义
const unsigned int MAX_THREAD_COUNTS = 100;
const unsigned int EPOLL_MAX_EVENTS = 1024;
const unsigned int MAX_SUBRCT_COUNTS = 20;

//* 枚举类
enum class IOType { BLOCK, NOBLOCK };

enum class DomainType { IPV4, IPV6 };

enum class ConnectionType { TCP_CONNECTION, UDP_CONNECTION };

class Error {
 public:
  static void error_check(bool state, std::string_view str) {
    if (!state) return;
    auto logger_ = spdlog::get("mainlogger");
    logger_->error(str);
    logger_->error(std::strerror(errno));
    exit(EXIT_FAILURE);
  }
};

class HelperFunc {
 public:
  static void setnoblock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
  }
};

};  // namespace MyTinyServer

#endif