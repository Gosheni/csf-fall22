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

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message msg;
  msg.tag = TAG_RLOGIN;
  msg.data = username;
  conn.send(msg);

  Message msg2;
  conn.receive(msg2);
  if (msg2.tag == TAG_ERR) {
    std::cout << msg2.tag << ":" << msg2.data << std::endl;
    return 1;
  } 

  Message msg3;
  msg3.tag = TAG_JOIN;
  msg3.data = room_name;
  conn.send(msg3);

  Message msg4;
  conn.receive(msg4);
  if (msg4.tag == TAG_ERR) {
    std::cout << msg4.tag << ":" << msg4.data << std::endl;
    return 1;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  while (conn.is_open()) {
    std::string m;
    std::cin >> m;
    int index = m.find(":");
    
    Message msg;
    msg.tag = m.substr(0, index);
    msg.data = m.substr(index+1);

    if (msg.tag == TAG_QUIT) break;

    Message msg2;
    conn.receive(msg2);
    std::cout << msg2.data << std::endl;
    if (msg2.tag != TAG_DELIVERY) return 1;

  }

  return 0;
}
