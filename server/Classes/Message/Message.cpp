#include "Message.hpp"

Message::Message(string content, string sender, ssize_t id) {
	_uid = id;
	gettimeofday(&_time, nullptr);
	_msg = content;
	_sender = sender;
}

bool	Message::compareTime(timeval time) {
	cerr << endl << time.tv_sec << " " << time.tv_usec << endl << " vs " << endl << _time.tv_sec << " " << _time.tv_usec << endl << endl;
	if (time.tv_sec < _time.tv_sec) {
		return false;
	} else if (time.tv_sec == _time.tv_sec && time.tv_usec < _time.tv_usec) {
		return false;
	}
	return true;
}

ssize_t	Message::getUid(void) {
	return _uid;
}

timeval	Message::getTime(void) {
	return _time;
}

string	Message::getContent(void) {
	string content = _sender;
	content += ": ";
	content += _msg;
	return content;
}
