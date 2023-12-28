#include "include/ThreadPool.h"

#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>

#include "util.h"

using namespace MyTinyServer;

ThreadPool::ThreadPool() : stop_(false) {
  for (int i = 1; i <= MAX_THREAD_COUNTS; ++i) {
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

void ThreadPool::addTask(std::function<void()>&& tk) {
  {
    std::unique_lock<std::mutex> task_lock(tasks_mutex_);

    if (stop_) {
      throw std::runtime_error("thread pool stoped, cannot add task");
    }

    tasks_.emplace(tk);
  }
  condition_var_.notify_one();
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