#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <iostream>
#include <memory>
#include <set>

#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

using namespace MyTinyServer;

void handleReadEvent(int sockfd) {
  char buf[1024];
  while (true) {
    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = recv(sockfd, buf, sizeof(buf), 0);
    if (bytes_read > 0) {
      printf("message from client fd %d: %s\n", sockfd, buf);
      send(sockfd, buf, sizeof(buf), 0);
    } else if (bytes_read == -1 && errno == EINTR) {
      //客户端正常中断、继续读取
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) ||
                (errno ==
                 EWOULDBLOCK))) {  //非阻塞IO，这个条件表示数据全部读取完毕
      std::cout << "finish reading once" << std::endl;
      break;
    } else if (bytes_read == 0) {  // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", sockfd);
      close(sockfd);  //关闭socket会自动将文件描述符从epoll树上移除
      break;
    }
  }
}

int main() {
  // * 创建服务器监听socket
  std::unique_ptr<Socket> socket(
      new Socket(DomainType::IPV4, ConnectionType::TCP_CONNECTION));

  InetAddress addr("127.0.0.1", 8888, DomainType::IPV4);

  socket->bind(addr);

  socket->listen(SOMAXCONN);

  // //* 创建epoll
  Epoll epoll;

  // 不能让它被析构掉，不然在epoll里面的fd就无效了
  Socket* tempsc = nullptr;
  socket->setnoblock();
  epoll.add_to_epoll(socket->fd(), EPOLLIN | EPOLLET);

  while (true) {
    //阻塞式轮询
    auto act_events = epoll.poll(-1);

    for (auto& act_event : act_events) {
      if (act_event.data.fd == socket->fd()) {
        InetAddress new_addr;
        int new_fd = socket->accept(&new_addr);
        // 离开作用域会被析构掉！！！
        tempsc = new Socket(new_fd);
        tempsc->setnoblock();
        epoll.add_to_epoll(tempsc->fd(), EPOLLIN | EPOLLET);
        std::cout << "new connection fd = " << tempsc->fd()
                  << " address = " << new_addr.readable_ipv4_address()
                  << std::endl;
      } else if (act_event.events & EPOLLIN) {
        handleReadEvent(act_event.data.fd);
      } else {
      }
    }
  }

  return 0;
}
