Jacob Choi - echoi51
Darren Shih - dshih5

Assignment 5 - Milestone 1
Jacob, Darren - We met and worked on constructing the base for Assignment 5 chat server
project. We implemented the code for connection server, the sender client, and the receiver 
client. After that, we discussed about our bugs and fixed the bugs together.

Assignment 5 - Milestone 2
Jacob, Darren - We met and worked on completing the server part for Assignment 5 chat server
project. We implemented the code for the server part. After that, we discussed about our bugs 
and fixed the bugs together.

Synchronization Report
In this assignment, we have identified 5 cricital sections (MessageQueue::enqueue, MessageQueue::dequeue, Room::add_member,
Room::remove_member, and Room::broadcast_message). Understanding that a critical region is a segment of code in which 
mutual exclusion must be guaranteed, we allowed our chat server to use threads for concurrency within a single space and 
use Guards to make sure that at most one concurrent task is executed at any given time. With this knowledge, we searched for 
the functions (or segments of code) in which multiple access may happen and has a potential to produce an unwanted behavior.

In message_queue.cpp, we decided to use guard.h for enqueue and dequeue functions, which is an alternative approach to locking
and unlocking a mutex. Because enqueuing and dequeuing a message into a message queue can happen simultaneously with the possibility
of multiple senders and receivers in a single room, unexpected behaviors may happen. As enqueuing and dequeuing are not atomic, 
we used guard.h to ensure that mutual exclusiveness occur in this multithread chat server.

In room.cpp, we allowed add_member, remove_member, and broadcast_message functions to use guard.h. In case of the add_member 
and remove_member functions, a similar approach to enqueue and dequeue is executed as described previously. Because the functions are 
not atomic, unexpected outcomes (i.e. when two thread calls add_member at the same time, there is a chance that only one member gets 
added to the room) may happen when mutual exclusion is not guaranteed. In case of the broadcast_message function, we enqueue a 
message to every single receiver in the room. If the function is called concurrently even with add_member or remove_member, the 
message may not be enqueued to some user, resulting some users to be left out.

As 5 functions mentioned above require mutual exclusion, we used guard.h to ensure a mutex is locked. Guard.h is used instead 
of manually calling pthread_mutex_lock and pthread_mutex_unlock as guard.h guarantees that the mutex will be released, which 
avoids deadlocks that may otherwise occur. Race conditions can be avoided as guard.h guarantees that at most one concurrent task 
is executed, instead of allowing a race for the data. 

In addition to the use of mutex, we used semaphores to manage access to a limited resource, which are messages inside the 
message_queue in our chat server. Messages inside user's message queue are not infinite. Thus, we use sem_post in 
MessageQueue::enqueue to increment our semaphore m_avail and use sem_wait (sem_timedwait to be more specific) in 
MessageQueue::dequeue to decrememt our semaphore m_avail. This, for example, allows the server to wait until the message
queue actually has at least one message (then decrements to indicate the message is sent). 