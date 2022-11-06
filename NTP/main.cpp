#include "ntp_client.hpp"

#include "time.h"
#include <iomanip>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return EXIT_FAILURE;
  }

  uint16_t port = std::stoi(argv[2]);

  NTPClient client{argv[1], port};

  auto dift = client.request_time();

  if (0 == dift)
    return EXIT_FAILURE;

  std::cout << "dift: " << dift << std::endl;

  return EXIT_SUCCESS;
}