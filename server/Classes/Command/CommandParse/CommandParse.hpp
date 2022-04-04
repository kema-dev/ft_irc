#ifndef _COMMANDPARSE_HPP
 #define _COMMANDPARSE_HPP

#include "../Command/Command.hpp"
#include "../../User/User.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

class CommandParse {
	private:

	public:
		CommandParse(void);
		~CommandParse(void);

		void	Join(void);
		void	Part(string message, string *channel_s, string *msg);
		void	Topic(string message, string *channel_s, string *topic);
		void	Privmsg(string message, string *channel_s, string *msg);
		void	Oper(string message, string* password);
		void	Kick(string message, string* chan, string* user, string* kickMsg, Server* serv);
		void	Mode(string message, string* user, int* req_op, int* req_away);
};

#endif