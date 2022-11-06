#ifndef SOCKET_H_
#define SOCKET_H_

#include "SocketDefs.h"
#include <iostream>
#include <string.h>
#include <string>

class Socket {
public:
  static bool createSocket(SocketData &s);
  static bool bind(SocketData &s);
  static bool listen(SocketData &s);
  static bool accept(SocketData &oldSock, SocketData &newSock);
  static bool setSocketOptions(SocketData &s);

  static bool connect(SocketData &s);

  static bool sendData(SocketData &s, std::string sData);
  static bool recvData(SocketData &s, std::string &sData);

  static bool isValidSocket(int &sockFd);
};

#endif /* SOCKET_H_ */
