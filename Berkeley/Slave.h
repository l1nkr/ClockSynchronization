
#ifndef SLAVE_H_
#define SLAVE_H_

#include "Socket.h"
#include "SocketDefs.h"
#include <fstream>
#include <iostream>
#include <map>
#include <pthread.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

const int MAXLINES = 500;

class Slaves {
  std::string sLocalTimes;
  std::string sName;
  std::string sPort;
  SocketData socket;

public:
  std::vector<std::string> StringSplitByDelim(std::string str, char &cDeLim);

  bool setPortNum(std::string sPort);
  bool setName(std::string sName);

  bool createSlaves();
  bool connectWithMaster();
};

#endif /* SERVER_H_ */
