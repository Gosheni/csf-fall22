#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Connection Failed!\n";
    return 1;
  }

  // TODO: send slogin message
  Message msg;
  msg.tag = TAG_SLOGIN;
  msg.data = username;
  if (!conn.send(msg)) {
    std::cout << msg.tag << ":" << msg.data << std::endl;
    std::cerr << "Error connecting to server\n";
    return 1;
  }

  Message msg2;
  if (!conn.receive(msg2)) {
    std::cout << msg2.tag << ":" << msg2.data << std::endl;
    std::cerr << "Error connecting to server\n";
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while (conn.is_open()) {
    std::string m;
    std::cin >> m;
    int index = m.find(":");
    
    Message msg;
    msg.tag = m.substr(0, index);
    msg.data = m.substr(index+1);

    if (msg.tag == TAG_QUIT) break;
  }

  return 0;
}
