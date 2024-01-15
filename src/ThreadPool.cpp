#include "include/ThreadPool.h"

#include <algorithm>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>

#include "Util.h"

using namespace MyTinyServer;

ThreadPool::ThreadPool(unsigned int sz) : stop_(false), size_(sz) {
  for (int i = 1; i <= std::min(sz, MAX_THREAD_COUNTS); ++i) {
    threads_.emplace_back(std::thread([this] {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> task_lock(tasks_mutex_);
          condition_var_.wait(task_lock,
                              [this] { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty()) return;
          task = tasks_.front();
          tasks_.pop();
        }
        //* 工作线程执行任务
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> task_lock(tasks_mutex_);
    stop_ = true;
  }
  condition_var_.notify_all();
  for (auto& thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}