#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <iostream>

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  std::stringstream str1;
  str1 << port;
  std::string buffer = str1.str();
  m_fd = Open_clientfd(hostname.c_str(), buffer.c_str());
  // TODO: call rio_readinitb to initialize the rio_t object
  if (m_fd >= 0) rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (is_open()) close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (is_open()) {
    Close(m_fd);
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  assert(is_open());
  std::stringstream ss;
  ss << msg.tag + ":" + msg.data + "\n";
  std::string n = ss.str();
  if (rio_writen(m_fd, n.c_str(), n.size()) != (ssize_t) n.size()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  } else {
    m_last_result = SUCCESS;
    return true;
  }
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  assert(is_open());
  char buf[Message::MAX_LEN+1];
  if (rio_readlineb(&m_fdbuf, buf, Message::MAX_LEN) < 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  std::string str(buf);
  auto index = str.find(":");
  if (index == std::string::npos) {
    m_last_result = INVALID_MSG;
    return false;
  }

  msg.tag = str.substr(0, index);
  msg.data = str.substr(index+1);

  if (!msg.is_lowercase()) {
    m_last_result = INVALID_MSG;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}