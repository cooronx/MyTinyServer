#ifndef SOCKET_H
#define SOCKET_H

#include <cstring>
#include <string>

#include "InetAddress.h"
#include "Noncopyable.h"
#include "Util.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "unistd.h"

using std::string;

namespace MyTinyServer {

/*
 * 注意Socket类的析构函数并不一定会关闭连接，只是减小引用计数
 * Socket 表示的是一个套接字，并不觉得它可以复制
 */
class Socket : Noncopyable {
 public:
  explicit Socket(DomainType, ConnectionType);
  Socket(int);
  ~Socket();

  // 将指定地址绑定到socket上面，一般用于服务器在某个ip地址端口上提供服务
  void bind(const InetAddress &);

  // 开始监听
  void listen(int);

  // // 设置为非阻塞socket
  // void setnoblock();

  //* 仅用于客户端socket
  void connect(InetAddress *);

  // *返回成功建立连接的fd
  // *需要传入一个空地址，会把建立连接的客户端地址写入这个空地址
  // !这里需要对传入的addr进行修改，所以不是const
  [[nodiscard]] int accept(InetAddress *);

  //强制转换为int的时候返回fd
  operator int() { return socket_; }
  [[nodiscard]] int fd() const { return socket_; }

 private:
  int socket_;
  DomainType domain_;
  ConnectionType conn_;
};

}  // namespace MyTinyServer

#endif  // SOCKET_H