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

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  try {  
    Message msg;
    msg.tag = TAG_SLOGIN;
    msg.data = username;
    send(conn, msg);
    receive(conn, msg, true);

    for (;;) {
      receive(conn, msg, true);
      if (msg.tag == TAG_QUIT) break;
    } 
  } catch (const std::runtime_error &ex) {
    throw("Error");
    conn.close();
    return 1;
  }

  return 0;
}
