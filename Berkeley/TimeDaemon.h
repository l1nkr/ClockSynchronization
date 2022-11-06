#ifndef TIMEDAEMON_H_
#define TIMEDAEMON_H_

#include "Socket.h"
#include "SocketDefs.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <pthread.h>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>
#include <sys/time.h>
class TimeDaemon {
  SocketData socketData;
  int sDaemonTime;
  int iAverage;
  int iAdjustedDaemonTime;

  std::string sInputFile;
  std::vector<SocketData> vSockets;
  std::vector<std::string> vNames;
  std::vector<std::string> vPorts;
  std::vector<std::string> vLocalTimes;
  std::vector<std::string> vOffsets;
  std::vector<int> vAdjustedDrifts;

public:
  bool readInputFile(std::string sFile);
  int startMaster();
  void setDaemonTime();
  void calculateDrift();

  std::vector<std::string> StringSplitByDelim(std::string str, char &cDeLim);
};

#endif /* TIMEDAEMON_H_ */
