#ifndef _USER_HPP
 #define _USER_HPP

class Server;

#include "../Server/Server.hpp"

#include <sys/types.h>
#include <iostream>

using namespace std;

class WrongUserName : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Username should only contain alphanumeric characters");
		}
};

class WrongNickName : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Nickname should only contain alphanumeric characters");
		}
};

class WrongFullName : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Fullname should only contain alphanumeric characters");
		}
};

class WrongRoleNameUser : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Role should either be 'user' or 'operator'");
		}
};

class BadRole : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User is not an operator");
		}
};

class NotLogged : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User is not logged to the channel");
		}
};

class NotLoggedGlobal : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User is not logged to the server");
		}
};

class AlreadyLogged : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User is already logged in to the server");
		}
};

class SameInfo : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Requested user infos are already in UserDB");
		}
};

class BadPasswd : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Given password is incorrect");
		}
};

class User {
	private:
	Server*		_server;
	string 		_username;
    string      _nickname;
    string      _fullname;
    string      _hostname;
    string      _servername;
	ssize_t		_nb_msg;
	bool		_ban_status;
	bool		_active_status;
	ssize_t		_uid;
	string		_hash;

	public:
	// User(string username, string fullname, string hostname, string servername, Server* server);
	User(string username, string fullname, string nickname, string hostname, string servername, Server* server);

	~User() {};
	
	string	getNickName(void);
	string	getUserName(void);
	string	getFullName(void);
	ssize_t	getNbMsg(void);
	bool	getBanStatus(void);
	bool	getActiveStatus(void);
	ssize_t	getUid(void);
	string	getHash(void);

	bool	setUserName(string new_username);
	bool	setNickName(string new_nickname);
	bool	setFullName(string new_fullname);
	bool	setNbMsg(ssize_t new_nb_msg);
	bool	setBanStatus(bool new_ban_status);
	bool	setActiveStatus(bool new_active_status);
	bool	setUid(ssize_t new_uid);
	bool	setHash(string new_hash);
	bool	setPass(string new_pass);

	bool	logIn(Server& server);
	bool	logOut(Server& server);
	void	sendMessage(string content, Channel& chan);
	void	joinChannel(Channel& chan, string pass);
	bool	ban(User& usr, Channel& chan);
	void	getBanned(Channel& chan, User& banner);
	bool	setPasswd(Channel& chan, string pass);
	bool	setOperPasswd(Channel& chan, string pass);
	bool	becomeOper(Channel& chan, string pass);
};

// std::ostream &	operator<<(std::ostream &stream, User &rhs);

#endif