#ifndef _COMMANDEXEC_HPP
 #define _COMMANDEXEC_HPP

#include "../Command/Command.hpp"
#include "../../User/User.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

class CommandExec {
	private:

	public:
		CommandExec(void);
		~CommandExec(void);

		void	join(User* user, vector<string> args);
		void	part(User* user, vector<string> args);
		void	quit(User* user, vector<string> args);
		void	nick(User* user, vector<string> args);
		void	topic(User *user, vector<string> args);
		void	privmsg(User* user, vector<string> args);
		void	welcome(User* user);
};

#endif