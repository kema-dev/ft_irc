#ifndef _COMMAND_HPP
 #define _COMMAND_HPP

#include <sys/types.h>
#include <iostream>
#include <vector>
using namespace std;

#include "../CommandExec/CommandExec.hpp"
#include "../CommandParse/CommandParse.hpp"
#include "../../Server/Server.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Error/Error.hpp"
#include "../../Reply/Reply.hpp"



class NeedMoreParams : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Not enough parameters");
		}
};

class UnknownParam : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Unknown parameter");
		}
};

class UnknownCommand : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Unknown command");
		}
};

class ChanOpeNeeded : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Operator status needed");
		}
};

#define ACTION_ADD 1
#define ACTION_REMOVE -1

// ? Commands
#define CMD_JOIN	0
#define CMD_PART	1
#define CMD_QUIT	2
#define CMD_NICK	3
#define CMD_USER	4
#define CMD_PONG	5
#define CMD_PRIVMSG	6
#define CMD_TOPIC	7
#define CMD_OPER	8
#define CMD_KICK	9
#define CMD_MODE	10
#define CMD_AWAY	11
#define CMD_NAMES	12
#define CMD_LIST 	13
#define CMD_NOTICE	14
#define CMD_PASS	15
#define CMD_WELCOME	16

class Command {
	private:
		string			_command;
		vector<string>	_args_v;
		vector<string>	_commands_v;
		CommandParse	_parse;
		CommandExec		_exec;

	public:
		Command(void);
		~Command(void);

		void	select(string smessage, User* user);
		void	parse(string smessage);
		void	welcome(User* user);
};

#endif