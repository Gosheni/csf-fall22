#include <iostream>
#include <string>
#include "connection.h"
#include "message.h"
#include "client_util.h"

// string trim functions shamelessly stolen from
// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

void send(Connection &conn, Message &msg) {
  if (!conn.send(msg)) {
    throw("Error");
  }
}

void receive(Connection &conn, Message &msg, bool throw_err) {
  if (!conn.receive(msg)) {
    if (conn.get_last_result() == Connection::INVALID_MSG) {
      throw("Invalid message");
    } else {
      throw("Server could not receive message");
    }
  }
  if (msg.tag == TAG_ERR && throw_err) {
    throw("Error");
  } 
}
