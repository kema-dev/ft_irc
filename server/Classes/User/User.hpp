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

	void	setUserName(string new_username);
	void	setNickName(string new_nickname);
	void	setFullName(string new_fullname);
	void	setNbMsg(ssize_t new_nb_msg);
	void	setBanStatus(bool new_ban_status);
	void	setActiveStatus(bool new_active_status);
	void	setUid(ssize_t new_uid);
	void	setHash(string new_hash);
	void	setPass(string new_pass);

	void	logIn(Server& server);
	void	logOut(Server& server);
	void	sendMessage(string content, Channel& chan);
	void	joinChannel(Channel& chan, string pass);
	void	ban(User& usr, Channel& chan);
	void	getBanned(Channel& chan, User& banner);
	void	setPasswd(Channel& chan, string pass);
	void	setOperPasswd(Channel& chan, string pass);
	void	becomeOper(Channel& chan, string pass);
};

// std::ostream &	operator<<(std::ostream &stream, User &rhs);

#endif