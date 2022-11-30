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
    std::cerr << "Open_clientfd error: Connection refused\n";
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
  } catch (const std::runtime_error &ex) {
    std::cerr << ex.what();
    conn.close();
    return 1;
  }
  
  for (;;) {
    std::cout << "> ";
    std::cout.flush();

    std::string buf;
    std::getline(std::cin, buf);

    Message msg2;

    if (buf.compare("/quit") == 0) {
      msg2.tag = TAG_QUIT;
      send(conn, msg2);
      receive(conn, msg2, true);
      break;
    }
    else if (buf.compare("/leave") == 0) msg2.tag = TAG_LEAVE;
    else if (buf.substr(0, 6).compare("/join ") == 0) {
      msg2.tag = TAG_JOIN;
      msg2.data = buf.substr(6);
    } 
    else {
      msg2.tag = TAG_SENDALL;
      msg2.data = buf;
    }
    try {
      send(conn, msg2);
      receive(conn, msg2, true);
    } catch (const std::runtime_error &ex) {
      std::cerr << ex.what();
      continue;
    }
  } 
  
  conn.close();
  return 0;
}