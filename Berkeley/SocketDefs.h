#ifndef SOCKETDEFS_H_
#define SOCKETDEFS_H_

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 10;
#define MAXRECV 500

typedef struct _SocketData {
  int iSocket;
  int iPortNum;
  std::string sHostname;
  sockaddr_in sockAddr;

} SocketData;

#endif /* SOCKETDEFS_H_ */
