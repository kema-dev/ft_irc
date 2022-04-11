#ifndef _USER_HPP
 #define _USER_HPP

class Channel;
class User;

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
			return ("User is already logged in to the channel");
		}
};

class AlreadyLoggedGlobal : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User already logged to the server");
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

class NickExists : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Nickname already in use");
		}
};

#define NOT_CONNECTED	0
#define CONNECTED		1
#define WELCOME         2
#define BANNED			3
#define UNKNOWN			4
#define AWAY			5

class User {
	private:
	Server*		_server;
	string 		_username;
    string      _nickname;
    string      _fullname;
    string      _hostname;
    string      _servername;
	ssize_t		_nb_msg;
	int 		_active_status;
	ssize_t		_uid;
    int         _socket;
    bool        _connected;
	string		_away_msg;

	public:
	User(string username, string fullname, string nickname, string hostname, string servername, Server* server, int socket);
    User();
	User(User &user);
	~User() {};

	
	Server*	getServer(void);
    bool    getConnectStatus(void);
	string	getNickName(void);
	string	getUserName(void);
	string	getFullName(void);
    string  getHostName(void);
    string  getServerName(void);
	ssize_t	getNbMsg(void);
	bool	getBanStatus(void);
	int	    getActiveStatus(void);
	ssize_t	getUid(void);
	string	getHash(void);
    int     getSocket(void);
	string	getAwayMessage(void);

    void    setServer(Server *server);
	void	setUserName(string new_username);
	void	setNickName(string new_nickname);
	void	setFullName(string new_fullname);
    void    setHostName(string new_hostname);
    void    setServerName(string new_servername);
	void	setNbMsg(ssize_t new_nb_msg);
	void	setActiveStatus(int new_active_status);
	void	setUid(ssize_t new_uid);
    void    setSocket(int socket);
    void    setConnectStatus(bool connected);
	void	setAwayMessage(string msg);

	void	logIn(Server& server);
	void	logOut(Server& server, string msg);
	void	sendMessage(string content, Channel& chan);
	void	joinChannel(Channel& chan, string pass);
	void	tryJoinChannel(string name, string pass, string topic, Server* server);
	void	tryPartChannel(string name, string chan, Server* server);
	void	kick(User& usr, Channel& chan, string msg);
	void	getKicked(Channel& chan, User& banner, string msg);
	void	setPasswd(Server& serv, string pass);
	void	setOperPasswd(Server& serv, string pass);
	void	becomeOper(Server& serv, string pass);
};

// std::ostream &	operator<<(std::ostream &stream, User &rhs);

#endif