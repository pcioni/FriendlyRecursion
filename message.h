#ifndef _message_h_
#define _message_h_

#include <string>

// a promise to define this class later
class Person;

// ================================================================
// Messages are passed between people.  A person can only pass a
// message to someone they consider to be their friend.  A message
// thus has a current owner.

class Message {

public:

  // CONSTRUCTOR
  Message(const std::string &msg, Person *person) { m_message = msg;
																	 m_owner = person; 
																	 sent = false;    }
  
  // ACCESSORS
  const std::string& get_message() const { return m_message; }
  Person* get_owner() const { return m_owner; }
  bool get_status() const { return sent; }

  // MODIFIER 
  // The message is transferred to the specified new owner.
  void pass_message(Person *person) { m_owner = person; sent = true; }
  //used for forwarding messages only one time per pass
  void reset_m() { sent = false; }
  void whisper();

private:

  // REPRESENTATION
  bool sent; //for passing messages; used to prevent passing a message more than once
  Person* m_owner;
  std::string m_message;

};

// ================================================================


#endif
