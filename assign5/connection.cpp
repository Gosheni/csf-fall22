#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

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
  char buffer[256];
  sprintf(buffer, "%d", port);
  m_fd = open_clientfd(hostname.c_str(), buffer);
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  if (m_fd < 0) return false;
  return true;
}

void Connection::close() {
  // TODO: close the connection if it is open
  Close(m_fd);
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string n = msg.data;
  if (rio_writen(m_fd, m_fdbuf.rio_bufptr, n.length()) != (int)n.length()) {
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
  if (rio_readlineb(&m_fdbuf, m_fdbuf.rio_bufptr, msg.MAX_LEN) < 0) {
    m_last_result = EOF_OR_ERROR;
    msg.data = m_fdbuf.rio_buf;
    msg.tag = TAG_ERR;
    return false;
  } else {
    m_last_result = SUCCESS;
    msg.data = m_fdbuf.rio_buf;
    msg.tag = TAG_OK;
    return true;
  }
}
