#include "include/Server.h"

#include <functional>

#include "Socket.h"
#include "util.h"

using namespace MyTinyServer;
using std::function;

Server::Server(EventLoop *loop) : eventloop_(loop) {
  //* server_socket 阻塞进行连接 使用LT方式触发
  server_socket_ = new Socket(DomainType::IPV4, ConnectionType::TCP_CONNECTION);
  server_addr = new InetAddress("127.0.0.1", 8888, DomainType::IPV4);
  server_socket_->bind(*server_addr);
  server_socket_->listen(SOMAXCONN);

  server_channel = new Channel(server_socket_->fd(), eventloop_);
  function<void()> callback = [this] { NewConnection(); };
  server_channel->set_callback(callback);
  server_channel->EnableRead(IOType::BLOCK);
}

void Server::HandleReadEvent(int sockfd) {
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

void Server::NewConnection() {
  InetAddress client_addr;
  int new_fd = server_socket_->accept(&client_addr);
  auto new_client_socket = new Socket(new_fd);
  //* 新建立的连接设置为非阻塞 ET触发
  HelperFunc::setnoblock(new_client_socket->fd());

  auto client_channel = new Channel(new_fd, eventloop_);
  function<void()> callback = [this, new_fd] { HandleReadEvent(new_fd); };
  client_channel->set_callback(callback);
  client_channel->EnableRead(IOType::NOBLOCK);

  std::cout << "new connection fd = " << new_client_socket->fd()
            << " address = " << client_addr.readable_ipv4_address()
            << std::endl;
}