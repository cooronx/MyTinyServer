#include <iostream>
#include <stdexcept>
#include <thread>

class Test {
 public:
  bool stop = false;
  void haha() {
    if (stop) {
      throw std::runtime_error("this is a exception");
    }
  }
  ~Test() { std::cout << "执行了析构" << std::endl; }
};

int main() {
  try {
    Test io;
    io.stop = true;
    io.haha();
  } catch (...) {
  }

  return 0;
}