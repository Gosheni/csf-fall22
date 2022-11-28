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
      Message msg2;
      receive(conn, msg2, true);
      if (msg2.split_c().size() != 3) {
        std::cerr << "Other Error\n";
        break;
      }
      std::cout << msg2.split_c().at(1) << ": " << msg2.split_c().at(2);
    } 
  } catch (const std::runtime_error &ex) {
    std::cerr << "Error\n";
    conn.close();
    return 1;
  }
  conn.close();
  return 0;
}