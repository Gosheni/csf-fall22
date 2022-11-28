#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Connection Failed!\n";
    return 1;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  
  try {  
    Message msg;
    msg.tag = TAG_RLOGIN;
    msg.data = username;
    send(conn, msg);
    receive(conn, msg, true);

    msg.tag = TAG_JOIN;
    msg.data = room_name;
    send(conn, msg);
    receive(conn, msg, true);

    for (;;) {
      receive(conn, msg, true);
      if (msg.tag != TAG_DELIVERY) {
        throw("Error");
      }
      if (msg.split_c().size() != 3) {
        throw("Other Error");
      }
    } 
  } catch (const std::runtime_error &ex) {
    throw("Error");
    conn.close();
    return 1;
  }

  return 0;
}
