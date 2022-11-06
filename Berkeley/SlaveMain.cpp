#include "Slave.h"

int main(int argc, char *argv[]) {
  Slaves node;

  if (argc != 3) {
    std::cout << "\nCommand to run servers: ./server <port_number> "
                 "<server_name> <server_time>\n";
    return -1;
  }
  node.setPortNum(argv[1]);
  node.setName(argv[2]);
  node.createSlaves();
  node.connectWithMaster();

  std::cout << std::endl;
  return 0;
}
