#ifndef _SEND_HPP
 #define _SEND_HPP

#include <sys/types.h>
#include <iostream>

#include "../User/User.hpp"
#include "../Log/Log.hpp"
#include "../Command/Command/Command.hpp"

using namespace std;

#define HEADER_SERVER 1
#define HEADER_CLIENT 2

class   Send {
	private:

	public:
	Send() {};
	~Send() {};

	// char*	gen_reply(char* format, va_list args);
	void	reply(User* sender, User* receiver, int code, int header, string format, ...);
	void	send_to_client(string header, int socket, const char *msg);
};

#endif
