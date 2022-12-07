#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

struct ClientData {
  Connection *conn;
  Server *server;

  ClientData(Connection *conn, Server *server) {
    conn = conn;
    server = server;
  }

  ~ClientData() {
    delete conn;
  }
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

bool is_valid_login_msg(Message msg) {
  if (msg.tag != TAG_RLOGIN && msg.tag != TAG_SLOGIN) {
    return false;
  }
  if (msg.split_c().size() > 1) {
    return false;
  }
  return true;
}

bool is_valid_join_msg(Message msg) {
  if (msg.tag != TAG_JOIN) {
    return false;
  }
  if (msg.split_c().size() > 1) {
    return false;
  }
  return true;
}

void chat_w_receiver(User* user, Connection* conn, Server *server) {
  bool send_ok = true;

  Message msg;
  if (!conn->receive(msg)) {
    conn->send(Message(TAG_ERR, "Couldn't receive message!"));
    return;
  }
  if (!is_valid_join_msg(msg)) {
    conn->send(Message(TAG_ERR, "Invalid Message!"));
    return;
  }

  Room *room = server->find_or_create_room(msg.data);
  room->add_member(user);

  for (;;) {
    Message *msg = user->mqueue.dequeue();
    if (msg != nullptr) {
      if (!conn->send(*msg)) {
        send_ok = false;
      }
      delete msg;
    }
    if (!send_ok) break;
  }
}

void chat_w_sender(User* user, Connection* conn, Server *server) {
  for (;;) {
    Message msg;
    if (!conn->receive(msg)) {
      break;
    }      
    if (msg.data.length() >= Message::MAX_LEN) {
      conn->send(Message(TAG_ERR, "Message is too long!"));
    }

    Room *room;
    if (msg.tag == TAG_JOIN) {
      room = server->find_or_create_room(msg.data);
      room->add_member(user);
      conn->send(Message(TAG_OK, "welcome"));
    } else if (msg.tag == TAG_LEAVE) {
      if (room != nullptr) {
        room->remove_member(user);
        conn->send(Message(TAG_OK, "left room"));
      } else {
        conn->send(Message(TAG_ERR, "Room not joined!"));
      }
    } else if (msg.tag == TAG_SENDALL) {
      if (room != nullptr) {
        room->broadcast_message(user->username, msg.data);
        conn->send(Message(TAG_OK, "message sent"));
      } else {
        conn->send(Message(TAG_ERR, "Room not joined!"));
      }
    } else if (msg.tag == TAG_QUIT) {
      room->remove_member(user);
      conn->send(Message(TAG_OK, "bye!"));
      break;
    } else {
      conn->send(Message(TAG_ERR, "Invalid tag!"));
    }
  }
  return;
}

void *worker(void *arg) {
  pthread_detach(pthread_self());

  std::unique_ptr<ClientData> client_data(static_cast<ClientData*>(arg));

  Connection* conn = client_data->conn;
  Server* serv = client_data->server;

  Message msg;
  if (!conn->receive(msg)) {
    if (conn->get_last_result() == conn->INVALID_MSG) {
      conn->send(Message(TAG_ERR, "Invalid Message!"));
      return nullptr;
    }
  }
  if (!is_valid_login_msg(msg)){
    return nullptr;
  }
  
  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  User* user = new User(msg.data);
  conn->send(Message(TAG_OK, "Logged in as " + msg.data));

  if (msg.tag == TAG_RLOGIN) {
    chat_w_receiver(user, conn, serv);
  } else if (msg.tag == TAG_SLOGIN) {
    chat_w_sender(user, conn, serv);
  }

  delete user;
  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::stringstream str1;
  str1 << m_port;
  std::string buffer = str1.str();
  int rc = Open_listenfd(buffer.c_str());
  if (rc < 0) return false;
  else{
    m_ssock = rc;
    return true;
  } 
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (1) {
    int clientfd = Accept(m_ssock, NULL, NULL);
    Connection *conn = new Connection(clientfd);
    ClientData *info = new ClientData(conn, this);
    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, info) < 0) {
      std::cerr << "pthread_create failed";
      delete info;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard G(m_lock);
  
  Room *res;
  auto index = m_rooms.find(room_name);
  if (index == m_rooms.end()) {
    res = new Room(room_name);
    m_rooms.insert({room_name, res});
  } else {
    res = index->second;
  }
  return res;
}
