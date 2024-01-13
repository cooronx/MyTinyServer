#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace MyTinyServer {

//*
class ThreadPool {
 public:
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;

  ThreadPool(unsigned int);
  ~ThreadPool();

  [[nodiscard]] auto size() const { return size_; }

  template <class F, class... Args>
  auto addTask(F &&fun, Args &&... args)
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(fun), std::forward<Args>(args)...));

    std::future<return_type> ret = task->get_future();
    {
      std::unique_lock<std::mutex> lock(tasks_mutex_);

      if (stop_) {
        throw std::runtime_error(
            "ThreadPool already stopped, add task failed!");
      }

      tasks_.emplace([task] { (*task)(); });
    }
    condition_var_.notify_one();
    return ret;
  }

 private:
  // ------------------
  std::mutex tasks_mutex_;                   //* 任务队列的互斥量
  std::vector<std::thread> threads_;         //* 工作线程集合
  std::queue<std::function<void()>> tasks_;  //* 任务队列
  std::condition_variable condition_var_;
  bool stop_;  // *线程池是否停止工作(虽然感觉没啥用)
  unsigned int size_;
};

}  // namespace MyTinyServer

#endif  // THREAD_POOL_H