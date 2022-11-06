#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define SERVER_PORT 8888
#define BUFF_LEN 7

void handle_udp_msg(int fd) {

  char buf[BUFF_LEN];
  socklen_t len;
  int count;
  struct sockaddr_in clent_addr;
  while (1) {
    memset(buf, 0, BUFF_LEN);
    len = sizeof(clent_addr);
    count =
        recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr *)&clent_addr, &len);
    if (count == -1) {
      printf("recieve data fail!\n");
      return;
    }

    struct timeval concise;
    gettimeofday(&concise, NULL);
    std::string msg = std::to_string(concise.tv_usec);

    memset(buf, 0, BUFF_LEN);
    strcpy(buf, msg.c_str());
    std::cout << buf << std::endl;

    sendto(fd, buf, BUFF_LEN, 0, (struct sockaddr *)&clent_addr, len);
  }
}

int main(int argc, char *argv[]) {
  int server_fd, ret;
  struct sockaddr_in ser_addr;
  // AF_INET:IPV4;SOCK_DGRAM:UDP
  server_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_fd < 0) {
    printf("create socket fail!\n");
    return -1;
  }

  memset(&ser_addr, 0, sizeof(ser_addr));
  ser_addr.sin_family = AF_INET;
  // INADDR_ANY：本地地址
  ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  ser_addr.sin_port = htons(SERVER_PORT);

  ret = bind(server_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
  if (ret < 0) {
    printf("socket bind fail!\n");
    return -1;
  }

  handle_udp_msg(server_fd);
  close(server_fd);
  return 0;
}