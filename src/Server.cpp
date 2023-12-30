#include "include/Server.h"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <functional>
#include <mutex>

#include "Socket.h"
#include "spdlog/spdlog.h"
#include "util.h"

using namespace MyTinyServer;
using std::function;

static std::shared_ptr<spdlog::logger> logger_{};

Server::Server(EventLoop *loop) : eventloop_(loop) {
  try {
    //* 初始化日志系统
    logger_ = spdlog::basic_logger_mt("mainlogger", "../../logs/a.log", true);
    logger_->flush_on(spdlog::level::info);
    logger_->info("日志系统启动成功");
    logger_->enable_backtrace(12);
  } catch (const spdlog::spdlog_ex &ex) {
    logger_->error("日志系统启动失败 {}", ex.what());
  }
  acceptor_ = new Acceptor(loop);
  function<void(Socket *)> cb = [this](auto &&PH1) {
    NewConnection(std::forward<decltype(PH1)>(PH1));
  };
  acceptor_->set_connection_callback(cb);
}

void Server::NewConnection(Socket *client_socket) {
  auto conn = new Connection(client_socket->fd(), eventloop_);
  function<void(int)> cb = [this](int fd) { DeleteConnection(fd); };
  conn->set_delete_conn_callback(cb);
  connections_[client_socket->fd()] = conn;
}

void Server::DeleteConnection(int fd) {
  {
    std::unique_lock<std::mutex> lock(mtx_);
    auto conn = connections_[fd];
    connections_.erase(fd);
    delete conn;
  }
}