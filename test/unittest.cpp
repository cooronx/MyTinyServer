#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <thread>

#include "Buffer.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

using namespace std;
using namespace MyTinyServer;

void oneClient(int msgs, int wait) {
  auto *sock = new Socket(DomainType::IPV4, ConnectionType::TCP_CONNECTION);
  auto *addr = new InetAddress("127.0.0.1", 1234, DomainType::IPV4);
  sock->connect(addr);

  int sockfd = sock->fd();

  auto *sendBuffer = new Buffer();
  auto *readBuffer = new Buffer();

  sleep(wait);
  int count = 0;
  while (count < msgs) {
    sendBuffer->append("I'm client!");
    ssize_t write_bytes =
        write(sockfd, sendBuffer->c_str(), sendBuffer->size());
    if (write_bytes == -1) {
      printf("socket already disconnected, can't write any more!\n");
      break;
    }
    int already_read = 0;
    char buf[1024];  //这个buf大小无所谓
    while (true) {
      bzero(&buf, sizeof(buf));
      ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
      if (read_bytes > 0) {
        readBuffer->append(buf);
        already_read += read_bytes;
      } else if (read_bytes == 0) {  // EOF
        printf("server disconnected!\n");
        exit(EXIT_SUCCESS);
      }
      if (already_read >= sendBuffer->size()) {
        printf("count: %d, message from server: %s\n", count++,
               readBuffer->c_str());
        break;
      }
    }
    readBuffer->clear();
  }
  delete addr;
  delete sock;
}

int main(int argc, char *argv[]) {
  int threads = 100;
  int msgs = 100;
  int wait = 0;
  int o;
  const char *optstring = "t:m:w:";
  while ((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 't':
        threads = stoi(optarg);
        break;
      case 'm':
        msgs = stoi(optarg);
        break;
      case 'w':
        wait = stoi(optarg);
        break;
      case '?':
        printf("error optopt: %c\n", optopt);
        printf("error opterr: %d\n", opterr);
        break;
    }
  }

  ThreadPool pool(1024);
  std::function<void()> func = [msgs, wait] { return oneClient(msgs, wait); };
  for (int i = 0; i < threads; ++i) {
    pool.addTask(func);
  }
  return 0;
}