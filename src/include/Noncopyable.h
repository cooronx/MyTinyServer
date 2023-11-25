#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace MyTinyServer {

class Noncopyable {
 protected:
  Noncopyable() = default;
  ~Noncopyable() = default;

 private:  // emphasize the following members are private
  Noncopyable(const Noncopyable&);
  const Noncopyable& operator=(const Noncopyable&);
};

}  // namespace MyTinyServer

#endif