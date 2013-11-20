#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include "third_party/cmdline.h"

struct server_config {
  server_config(int argc, char** argv) {
    cmdline::parser p;
    p.add<size_t>("threads", 't', "Thread Quantity", false, 4);
    p.add<size_t>("max-memory", 'M', "max usage of memory(MegaBytes)", false, 256);
    p.add<uint16_t>("port", 'p', "Use port", false, 11211);
    p.add<size_t>("sockets", 's', "CPU sockets", false, 1);
    p.add<size_t>("verbose", 'v', "Verbosity", false, 1);
  }
  size_t threads;
  size_t max_memory;  // MegaBytes
  uint16_t port;
  size_t cpu_sockets;
  size_t verbose;
};

#endif  // SERVER_CONFIG_HPP_
