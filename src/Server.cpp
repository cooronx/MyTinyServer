#include "include/Server.h"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <functional>
#include <mutex>

#include "Channel.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "Util.h"
#include "spdlog/spdlog.h"

using namespace MyTinyServer;
using std::function;

static std::shared_ptr<spdlog::logger> logger_{};

Server::Server(EventLoop *loop) : main_reactor_(loop) {
  try {
    //* 初始化日志系统
    logger_ = spdlog::basic_logger_mt("mainlogger", "../../logs/a.log", true);
    logger_->flush_on(spdlog::level::info);
    logger_->info("日志系统启动成功");
    logger_->enable_backtrace(12);
  } catch (const spdlog::spdlog_ex &ex) {
    logger_->error("日志系统启动失败 {}", ex.what());
  }

  thpool_ = new ThreadPool(MAX_SUBRCT_COUNTS);

  //* 主Reactor
  acceptor_ = new Acceptor(main_reactor_);
  function<void(Socket *)> cb = [this](auto &&PH1) {
    NewConnection(std::forward<decltype(PH1)>(PH1));
  };
  acceptor_->set_connection_callback(cb);

  //* 从属Reactor
  for (int i = 0; i < MAX_SUBRCT_COUNTS; ++i) {
    sub_reactors_.emplace_back(new EventLoop());
  }

  for (auto &rec : sub_reactors_) {
    auto subfunc = [rec] { rec->Loop(); };
    thpool_->addTask(subfunc);
  }
}

void Server::NewConnection(Socket *client_socket) {
  auto conn = new Connection(
      client_socket->fd(), sub_reactors_[RandomSubIndex(client_socket->fd())]);
  function<void(int)> cb = [this](int fd) { DeleteConnection(fd); };
  conn->set_delete_conn_callback(cb);
  connections_[client_socket->fd()] = conn;
}

void Server::DeleteConnection(int fd) {
  auto it = connections_.find(fd);
  if (it != connections_.end()) {
    auto conn = connections_[fd];
    connections_.erase(fd);
    delete conn;
  }
}
int Server::RandomSubIndex(int fd) { return fd % sub_reactors_.size(); }