#include "Message.hpp"

// ? Create a new message
Message::Message(string content, string sender, ssize_t id) {
	_uid = id;
	gettimeofday(&_time, nullptr);
	_msg = content;
	_sender = sender;
}

// ? Compare <time> with <this> creation time
bool	Message::compareTime(timeval time) {
	if (time.tv_sec < _time.tv_sec) {
		return true;
	} else if (time.tv_sec == _time.tv_sec && time.tv_usec < _time.tv_usec) {
		return true;
	}
	return false;
}

// ? Get UID of <this>
ssize_t	Message::getUid(void) {
	return _uid;
}

// ? Get time of creation of <this>
timeval	Message::getTime(void) {
	return _time;
}

// ? Get <this> content
string	Message::getContent(void) {
	return _sender + ": " + _msg;
}
