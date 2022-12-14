#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include "client_util.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  Guard G(lock);
  members.insert(user);
}

void Room::remove_member(User *user) {
  Guard G(lock);
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  Guard G(lock);

  for (std::set<User *>::iterator itr = members.begin(); itr != members.end(); itr++) {
    if((*itr)->username != sender_username){
      std::string msg = trim(room_name) + ":" + trim(sender_username) + ":" + trim(message_text);
      Message *m = new Message(TAG_DELIVERY, msg);
      (*itr)->mqueue.enqueue(m);
    }
  }
}