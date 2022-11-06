#ifndef TIMEDAEMONMAIN_CPP_
#define TIMEDAEMONMAIN_CPP_

#include "TimeDaemon.h"

int main(int argc, char *argv[]) {
  TimeDaemon master;
  srand((unsigned int)time(NULL));

  if (argc != 2) {
    std::cout << "\nCommand to run time daemon: ./daemon <input_file> \n";
    return -1;
  }

  master.readInputFile(argv[1]);
  // 不需要手动设定时间
  // master.setDaemonTime(argv[2]);
  int error = master.startMaster();
  std::cout << std::endl;
  std::cout << error << std::endl;
  return 0;
}

#endif /* COORDINATORMAIN_CPP_ */
