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
		void	welcome(User* user);
		// void	part(t_params *params, string channel_s, string msg);
		// void	topic(t_params *params, string args, string topic);
		// void	privateMessage(t_params *params, string args, string message);
		// void	oper(string message, t_params* params);
		// void	kick(string message, t_params* params);
		// void	mode(string message, t_params* params);
		// void	nick(t_params * params, string args);
		// void	user(t_params * params, string args);
};

#endif