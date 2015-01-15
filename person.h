#ifndef _person_h_
#define _person_h_

#include <list>
#include <string>

// a promise to define this class later
class Message;
#include "message.h"


// ================================================================
// The person class has a name a list of other people who this person
// considers to be friends.  Note: the friendship relationship is not
// necessarily bi-directional.  A person also has a list of messages.

class Person {
public:

  // CONSTRUCTOR
  Person(const std::string &name) { m_name = name; }
  ~Person();
  
  // ACCESSORS
  const std::string& get_name() const { return m_name; }
  const std::list<Person*>& get_friends() const { return m_friends; }
  const std::list<Message*>& get_messages() const { return m_messages; }
  bool is_friend(Person* person) const;
  void print_friends();
  void print_messages();

  // MODIFIERS
  // These functions return false if the operation was not
  // successfully completed.
  void add_friend(Person *person) { m_friends.push_back(person); }
  void remove_friend(Person* person) { m_friends.remove(person); }
  void add_message(Message *message) { m_messages.push_back(message); }
  bool remove_message(Message *message) { m_messages.remove(message); }
  void remove_self();
  void reset_p();

  
  
private:

  // REPRESENTATION
  std::string m_name;
  std::list<Person*> m_friends;
  std::list<Message*> m_messages;
};

// ================================================================

inline bool friend_sorter(const Person* a, const Person* b) {
  return (a->get_name() < b->get_name());
}

bool Person::is_friend(Person* person) const {
	std::list<Person*>::const_iterator persons;
	for (persons = m_friends.begin(); persons != m_friends.end(); ++persons) {
		if (*persons == person)
			return true;
	}
	return false;
}

//changes all the state of all a person's messages to "false"
void Person::reset_p() {
	std::list<Message*>::iterator messages;
	for (messages = m_messages.begin(); messages != m_messages.end(); ++ messages) 
		(*messages)->reset_m();
}

void Person::print_friends() {
	std::list<Person*>::iterator persons;
	std::set<std::string>::iterator name;
	std::cout << "  friends: ";
	std::set<std::string> names;
	for (persons = m_friends.begin(); persons != m_friends.end(); ++persons) 
		names.insert( (*persons)->get_name() );
	for (name = names.begin(); name != names.end(); ++name) 
		std::cout << *name << ' ';
	std::cout << std::endl;
}
//two self explanatory functions...iterate through the list, printing as you go
void Person::print_messages() {
	std::list<Message*>::iterator messages;
	std::set<std::string>::iterator message;
	std::cout << "  messages: ";
	std::set<std::string> message_set;
	//sets are automatically sorted, thus guaranteeing alphaebetcal order
	for (messages = m_messages.begin(); messages != m_messages.end(); ++ messages) 
		message_set.insert( (*messages)->get_message() );
	for (message = message_set.begin(); message != message_set.end(); ++message) 
		std::cout << *message << ' ';
	std::cout << std::endl;

}

Person::~Person() {
	std::list<Message*>::iterator messages;
	//only delete our messages since the person* need to stay intact. 
	for (messages = m_messages.begin(); messages != m_messages.end(); ++ messages)
		delete *messages; 
}


#endif
