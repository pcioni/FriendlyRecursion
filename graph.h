#ifndef _graph_h_
#define _graph_h_

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <set>
#include "MersenneTwister.h"

// a promise to define these classes later
class Person;
class Message;

// ================================================================
// A graph stores a vector of all the people currently logged on and a
// vector of all the messages in the system.  People are added and
// removed from the system.  Friendships are established between
// people.  Messages can be added and passed randomly within the
// network.  The current and complete state of the system can be 
// printed.  Alternatively, the invitation list of a certain depth of
// friends from a given person can also be printed.

class Graph {

public:
  // CONSTRUCTOR & DESTRUCTOR
  Graph() {};
  ~Graph();

  // ACCESSORS
  Person* find_person(const std::string &name) const;

  // MODIFIERS
  // These functions return false if the operation was not
  // successfully completed.
  bool add_person(const std::string& person_name);
  bool remove_person(const std::string& person_name);
  bool add_friendship(const std::string& person_name1, const std::string& person_name2);
  bool remove_friendship(const std::string& person_name1, const std::string& person_name2);
  bool add_message(const std::string& person_name, const std::string& message);
  void pass_messages(MTRand &mtrand);
  void reset();

  // OUTPUT
  void print(std::ostream &ostr) const;
  bool print_invite_list(std::ostream &ostr, const std::string &name, int n,  
								 std::set<std::string> &members, int current_moves) const;

private:

  // REPRESENTATION
  std::vector<Person*> m_people;
  std::vector<Message*> m_messages;
};

// ================================================================

Person* Graph::find_person(const std::string &name) const {
	for (int i = 0; i < m_people.size(); ++i) {
		if (m_people[i]->get_name() == name) 
			return m_people[i];
	}
	//NULL indicates the person isn't found
	return NULL;
}


bool Graph::add_person(const std::string& person_name) {
	if (find_person(person_name) != NULL)
		return false;
	//if the person doesnt exist, return false. Otherwise,
	//make a new Person object and add it to the vector.
	Person* tmp = new Person(person_name); 
	m_people.push_back(tmp);
	return true;
}

bool Graph::remove_person(const std::string& person_name) {
	if (find_person(person_name) == NULL) 
		return false;
		
	Person* perso = find_person(person_name); //identify our target person
	std::list<Person*>::iterator persons;
	for (int i = 0; i < m_people.size(); ++i) {
		if (m_people[i]->is_friend(perso))	//find the person and remove all friendships to/from them
			remove_friendship(m_people[i]->get_name(), perso->get_name());
	}
	
	std::vector<Person*>::iterator itr;
	for (itr = m_people.begin(); itr != m_people.end(); ++itr) {
		if (*itr == perso) 	
			m_people.erase(itr);	//also erase the person from our overhead list
	}
	
	delete perso; //and finally delete the object itself
	
	return true;
}

bool Graph::add_friendship(const std::string& person_name1, const std::string& person_name2) {
	if (find_person(person_name1) == NULL) 
		return false;
							//make sure both of the names exist.
	else if (find_person(person_name2) == NULL)
		return false;
		
	else if ( (find_person(person_name1) )->is_friend( find_person(person_name2) ) )
		return false;	//dont add two people who are already friends
	
		
	else {
		Person* perso = find_person(person_name1);
		Person* frien = find_person(person_name2); //friend is a reserved word, hence weird var names
		perso->add_friend(frien);		//add the friendship to the person object
		return true;
	}

}

bool Graph::remove_friendship(const std::string& person_name1, const std::string& person_name2) {
	if (find_person(person_name1) == NULL) 
		return false;
							//make sure both of the names exist.
	else if (find_person(person_name2) == NULL) 
		return false;
		
	else if ( !(find_person(person_name1) )->is_friend( find_person(person_name2) ) )
		return false; 	//dont remove two people who arent even friends
	
	else {
		Person* perso = find_person(person_name1);
		Person* frien = find_person(person_name2); 
		perso->remove_friend(frien);
		return true;
	}
	
}

void Graph::print(std::ostream &ostr) const {
	std::cout << std::endl;
	for (int i = 0; i < m_people.size(); ++i) {
		std::cout << (m_people[i])->get_name() << std::endl;;
		(m_people[i])->print_friends();
		(m_people[i])->print_messages(); //MAKE THESE ALPHAEBETICAL ///////////////////////////////
	}
	std::cout << std::endl;
}

bool Graph::add_message(const std::string& person_name, const std::string& message) {
	if (find_person(person_name) == NULL) 
		return false;	
	
	Person* perso = find_person(person_name);
	Message* tmp = new Message(message, perso); //find the target person, make a new message, and add it
	perso->add_message(tmp);
	return true;
}

void Graph::pass_messages(MTRand &mtrand) {
	int choices; //the total paths our messages can take
	int result;	 //our deciding number
	std::list<Message*> messages;
	std::list<Message*>::iterator messages_itr;
	for (int i = 0; i < m_people.size(); ++i) {
		messages = m_people[i]->get_messages();	//for each person, grab the message list 
		choices = ( m_people[i]->get_friends() ).size(); //+1 for a 'don't move' option
		for (messages_itr = messages.begin(); messages_itr != messages.end(); ++ messages_itr) {
			result = mtrand.randInt(choices + 100) % (choices + 1); //rand % choices results in a number 0-choices
			if ((*messages_itr)->get_status() == false) { //only pass a message once!
				if (result == choices)	//"don't move" option
					continue;
				else {	//set an iterator to our result index and mnuove the messages/reassign ownership
					std::list<Person*>::const_iterator persons = ( m_people[i]->get_friends() ).begin();
					std::advance(persons, result); 
					(*messages_itr)->pass_message(*persons);
					(*persons)->add_message(*messages_itr);
					m_people[i]->remove_message(*messages_itr);
					
				}
			}
		} 
	}
	reset();
}


bool Graph::print_invite_list(std::ostream &ostr, const std::string &name, int n, 
										std::set<std::string> &members, int current_moves = 0) const {
	if (find_person(name) == NULL) 
		return false;	
	else if (current_moves < n) {   				//if we can look deeper,
		Person* perso = find_person(name);		
		std::list<Person*>::iterator persons;
		std::list<Person*> friends = perso->get_friends();	//grab our list of friends and loop through
		for (persons = friends.begin(); persons != friends.end(); ++ persons) { 
			members.insert((*persons)->get_name());			//and add their degree of friends
			print_invite_list(ostr, (*persons)->get_name(), n, members, current_moves+1); 
		}
	}
	return true;
}

//initates the resetting process. Used for message passing 
void Graph::reset() {
	for (int i = 0; i < m_people.size(); ++i) 
		m_people[i]->reset_p();
}

Graph::~Graph(){
	for (int i = 0; i < m_people.size(); ++i) 
		delete m_people[i];
	

}

#endif
