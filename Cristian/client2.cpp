#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#define SERVER_PORT 8888
#define BUFF_LEN 7
// #define SERVER_IP "192.168.1.10"

void udp_msg_sender(int fd, struct sockaddr* dst) {

    socklen_t len;
    struct sockaddr_in src;
    while(1) {
        char buf[BUFF_LEN];

        // request_time
        struct timeval request_time;
        gettimeofday(&request_time, NULL);
        int request_msec = request_time.tv_usec;

        // server_time
        // 接受信息，解析服务器时间
        len = sizeof(*dst);
        sendto(fd, buf, BUFF_LEN, 0, dst, len);
        memset(buf, 0, BUFF_LEN);
        recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&src, &len);

        // response_time
        struct timeval response_time;
        gettimeofday(&response_time, NULL);
        int response_msec = response_time.tv_usec;

        // actual_time
        struct timeval actual_time;
        gettimeofday(&actual_time, NULL);
        int actual_msec = actual_time.tv_usec;

        // process_delay_latency
        int process_delay_latency = response_msec - request_msec;

        // 认为只有微秒级别误差
        // todo 可以使用stoi
        int server_msec = 0;
        for(int i = 0; i < BUFF_LEN-1; i++) {
            server_msec = server_msec * 10 + (buf[i] - '0');
        }

        int client_msec = server_msec + process_delay_latency / 2;

        int error = actual_msec - client_msec;

        std::cout << "request_msec" << request_msec << std::endl;
        std::cout << "server_msec" << server_msec << std::endl;
        std::cout << "actual_msec" << actual_msec << std::endl;
        std::cout << "process_delay_latency" << process_delay_latency <<std::endl;
        std::cout << "client_msec" << client_msec << std::endl;
        std::cout << "error" << error << std::endl;

        sleep(3);
    }
}

int main(int argc, char* argv[]) {
    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0) {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    // ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(SERVER_PORT);

    udp_msg_sender(client_fd, (struct sockaddr*)&ser_addr);
    close(client_fd);
    return 0;
}