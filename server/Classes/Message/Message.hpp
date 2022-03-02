#ifndef _MESSAGE_HPP_
 #define _MESSAGE_HPP_

#include <string>
#include <iostream>
#include <sys/time.h>

using namespace std;

class Message {
	private:
	ssize_t	_uid;
	timeval	_time;
	string	_msg;
	string	_sender;

	public:
	Message(string content, string sender, ssize_t id);
	~Message() {};

	bool	compareTime(timeval time);
	ssize_t	getUid(void);
	timeval	getTime(void);
	string	getContent(void); // ! ANCHOR add specific func to separate sender and contnt getters
};

#endif