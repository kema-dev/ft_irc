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

		void	parse_Join(void);
		void	parse_Part(string message, string *channel_s, string *msg);
		void	parse_Topic(string message, string *channel_s, string *topic);
		void	parse_Privmsg(string message, string *channel_s, string *msg);
		void	parse_Oper(string message, string* password);
		void	parse_Kick(string message, string* chan, string* user, string* kickMsg, Server* serv);
		void	parse_Mode(string message, string* user, int* req_op, int* req_away);
};

#endif