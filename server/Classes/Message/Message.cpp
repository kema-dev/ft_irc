#include "Message.hpp"

Message::Message(string content, string sender, ssize_t id) {
	_uid = id;
	gettimeofday(&_time, nullptr);
	_msg = content;
	_sender = sender;
}

bool	Message::compareTime(timeval time) {
	if (time.tv_sec < _time.tv_sec) {
		return true;
	} else if (time.tv_sec == _time.tv_sec && time.tv_usec < _time.tv_usec) {
		return true;
	}
	return false;
}

ssize_t	Message::getUid(void) {
	return _uid;
}

timeval	Message::getTime(void) {
	return _time;
}

string	Message::getContent(void) {
	return _sender + ": " + _msg;
}
