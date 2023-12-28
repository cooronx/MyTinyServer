#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace MyTinyServer {

//* 单例模式
class ThreadPool {
 public:
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;

  //* 用这个方法获取唯一的实例
  static ThreadPool &GetInstance() {
    static ThreadPool pool_;
    return pool_;
  }

  //----------------------
  void addTask(std::function<void()> &&);

 private:
  ThreadPool();
  ~ThreadPool();

  // ------------------
  std::mutex tasks_mutex_;                   //* 任务队列的互斥量
  std::vector<std::thread> threads_;         //* 工作线程集合
  std::queue<std::function<void()>> tasks_;  //* 任务队列
  std::condition_variable condition_var_;
  bool stop_;  // *线程池是否停止工作(虽然感觉没啥用)
};

}  // namespace MyTinyServer

#endif  // THREAD_POOL_H