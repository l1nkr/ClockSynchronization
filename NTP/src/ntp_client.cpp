#include "ntp_client.hpp"

#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define XTIME_SEC_1900_1970 0x83AA7E80

/** 百纳秒 的进位基数 10^7 */
#define XTIME_100NS_BASE 10000000ULL
#define XTIME_INVALID_UNSEC ((u_int64_t)~0ULL)

#define XTIME_STOU(xstamp, xunsec)                                             \
  do {                                                                         \
    if (((xstamp).xut_seconds) > XTIME_SEC_1900_1970)                          \
      (xunsec) =                                                               \
          (((xstamp).xut_seconds - XTIME_SEC_1900_1970) * XTIME_100NS_BASE) +  \
          (((xstamp).xut_fraction * XTIME_100NS_BASE) >> 32);                  \
    else                                                                       \
      (xunsec) = XTIME_INVALID_UNSEC;                                          \
  } while (0)

typedef struct xtime_stamp_t {
  u_int32_t xut_seconds; ///< 从 1900年至今所经过的秒数
  u_int32_t xut_fraction; ///< 小数部份，其单位是 百纳秒数 的 (2^32 / 10^7) 倍
} xtime_stamp_t;

NTPClient::NTPClient(std::string hostname, uint16_t port)
    : hostname_(hostname), port_(port) {}

void NTPClient::build_connection() {
  // Creating socket file descriptor
  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    std::cerr << "Socket creation failed\n";
    return;
  }

  memset(&socket_client, 0, sizeof(socket_client));

  std::string ntp_server_ip = hostname_to_ip(hostname_);

  std::cout << "Creating socket with: " << ntp_server_ip << "\n";

  timeval timeout_time_value{};
  timeout_time_value.tv_sec = 1; // timeout in seconds
  setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO,
             (const char *)&timeout_time_value, sizeof(timeout_time_value));

  // Filling server information
  socket_client.sin_family = AF_INET;
  socket_client.sin_port = htons(port_);
  socket_client.sin_addr.s_addr = inet_addr(ntp_server_ip.c_str());
}

NTPClient::~NTPClient() { close(socket_fd); }

uint64_t NTPClient::request_time() {
  int response; // return result from writing/reading from the socket

  build_connection();

  std::cout << "Connecting\n";
  if (connect(socket_fd, (struct sockaddr *)&socket_client,
              sizeof(socket_client)) < 0) {
    std::cerr << "ERROR connecting\n";
    return 0;
  }

  NTPPacket packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  packet.li_vn_mode = 0x1b;

  struct timeval request_time;
  gettimeofday(&request_time, NULL);
  std::cout << request_time.tv_sec << " " << request_time.tv_usec << std::endl;

  std::cout << "Sending request\n";
  response = write(socket_fd, (char *)&packet, sizeof(NTPPacket));

  if (response < 0) {
    std::cerr << "ERROR writing to socket\n";
    return 0;
  }

  std::cout << "Reading request\n";
  response = read(socket_fd, (char *)&packet, sizeof(NTPPacket));

  struct timeval response_time;
  gettimeofday(&response_time, NULL);
  std::cout << response_time.tv_sec << " " << response_time.tv_usec
            << std::endl;

  if (response < 0) {
    std::cerr << "ERROR reading from socket\n";
    close_socket();
    return 0;
  }
  //  std::cout << "--- debug  ---" << std::endl;

  packet.received_timestamp_sec = ntohl(packet.received_timestamp_sec);
  // Time-stamp seconds.
  packet.received_timestamp_sec_frac =
      ntohl(packet.received_timestamp_sec_frac);
  // Time-stamp fraction of a second.
  packet.transmited_timestamp_sec = ntohl(packet.transmited_timestamp_sec);
  // Time-stamp seconds.
  packet.transmited_timestamp_sec_frac =
      ntohl(packet.transmited_timestamp_sec_frac);

  xtime_stamp_t t1;

  t1.xut_seconds = packet.received_timestamp_sec;
  t1.xut_fraction = packet.received_timestamp_sec_frac;
  int t1_i = 0;
  XTIME_STOU(t1, t1_i);
  t1_i = t1_i / 100;

  xtime_stamp_t t2;
  t2.xut_seconds = packet.transmited_timestamp_sec;
  t2.xut_fraction = packet.transmited_timestamp_sec_frac;
  int t2_i = 0;
  XTIME_STOU(t2, t2_i);
  t2_i = t2_i / 100;
  u_int32_t t0_i = request_time.tv_usec;
  u_int32_t t3_i = response_time.tv_usec;
  int delay = ((t3_i - t0_i) - (t2_i - t1_i)) / 2;
  int dift = (t2_i / 100 + delay - t3_i) / 100000000;

  return dift;
}

std::string NTPClient::hostname_to_ip(const std::string &host) {
  hostent *hostname = gethostbyname(host.c_str());
  if (hostname)
    return std::string(inet_ntoa(**(in_addr **)hostname->h_addr_list));
  return {};
}

void NTPClient::close_socket() {
  if (socket_fd != -1) {
    close(socket_fd);
    socket_fd = -1;
  }
}