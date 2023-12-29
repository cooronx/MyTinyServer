#ifndef BUFFER_H
#define BUFFER_H

#include <sys/types.h>

#include <string>

namespace MyTinyServer {

using std::string;

class Buffer {
 public:
  Buffer(const char *str) { content_ = static_cast<string>(str); }
  Buffer() = default;

  //* 向content中添加内容
  void append(const char *str) { content_.append(str); }

  //* 包装函数
  [[nodiscard]] const char *c_str() const { return content_.c_str(); }
  [[nodiscard]] size_t size() const { return content_.size(); }
  void clear() { content_.clear(); }

 private:
  string content_;
};

}  // namespace MyTinyServer

#endif  // BUFFER_H