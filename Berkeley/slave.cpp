#include "Slave.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <pthread.h>
#include <sstream>
#include <string>
#include <vector>
#include <sys/time.h>

using std::cout;
using std::map;
using std::string;
using std::vector;

#define MAX_NUM_OF_SECONDS (5 * 365 * 24 * 60 * 60)

// todo
// 获取更精确的时间
struct tm *GetTimeAndDate() {
  unsigned int now_seconds = (unsigned int)time(NULL);
  unsigned int rand_seconds = (rand() * rand()) % (MAX_NUM_OF_SECONDS + 1);
  time_t rand_time = (time_t)(now_seconds - rand_seconds);
  return localtime(&rand_time);
}

bool Slaves::setPortNum(string sPort) {
  bool res = true;
  this->sPort = sPort;
  return res;
}

bool Slaves::setName(string sName) {
  bool res = true;
  this->sName = sName;
  return res;
}

bool Slaves::createSlaves() {
  bool res = true;
  socket.iPortNum = stoi(sPort);
  if (!Socket::createSocket(socket)) {
    cout << "\nError in " << __func__;
    return false;
  }
  if (!Socket::bind(socket)) {
    cout << "\nError in " << __func__;
    return false;
  }
  if (!Socket::listen(socket)) {
    cout << "\nError in " << __func__;
    return false;
  }
  return res;
}

bool Slaves::connectWithMaster() {
  bool res = true;

  SocketData *newSocket = new SocketData();
  newSocket->iPortNum = socket.iPortNum;
  newSocket->sHostname = socket.sHostname;
  Socket::accept(socket, *newSocket);

  string message = "from " + sName;
  Socket::sendData(*newSocket, message);

  string sMasterTime;
  srand(socket.iPortNum);
  
  // 获取服务器的时间
  Socket::recvData(*newSocket, sMasterTime);

  // 原时间
  struct timeval raw_time;
  gettimeofday(&raw_time, NULL);
  int raw_msec = raw_time.tv_usec;

  // 获得响应时间
  struct timeval response_time;
  gettimeofday(&response_time, NULL);
  int response_msec = response_time.tv_usec;

  // 计算此时服务器的时间
  int master_msec = stoi(sMasterTime);
  int master_msec_true = master_msec + (response_msec - raw_msec) / 2;

  // 计算漂移
  int dift = master_msec_true - response_msec;

  cout << "\n[" << sName << "] dift : " << master_msec_true;
  // 
  string sDift = std::to_string(dift);
  // string sDiff = std::to_string(iMyTime - stoi(sMasterTime));
  // cout << "\n[" << sName << "] Slave clock is: " << iMyTime;

  Socket::sendData(*newSocket, sDift);

  // 获取修正后的时间
  string corr;
  Socket::recvData(*newSocket, corr);
  cout << "\n[" << sName << "] Offset for server is: " << corr;
  // cout << "\n[" << sName << "] Adjusted time: " << iMyTime + stoi(sDiff) << std::endl;
  return res;
}
