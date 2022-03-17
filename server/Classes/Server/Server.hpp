#ifndef _SERVER_HPP
 #define _SERVER_HPP

class ChannelDB;

#include "../UidPool/UidPool.hpp"
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"

#include <sys/types.h>
#include <iostream>

using namespace std;

typedef struct s_params
{
    int client_socket;
    Server *irc_serv;
    ssize_t user_id;
}   t_params;

class ServerFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Server creation failed");
		}
};

class ChanAddFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Channel adding failed");
		}
};

class UserAddFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User adding failed");
		}
};

class ChanDuplicate : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Can't create channel (duplicate)");
		}
};

class UserDuplicate : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Can't create user (duplicate)");
		}
};

class Server {
	private:
	string		_hash;

	public:
	string		name;
	UidPool*	pool;
	ChannelDB*	chanDB;
	UserDB*		userDB;

	public:
	Server(string dname, string pass);
	~Server();

    string  getHash();
	void	addChan(string name, string pass, string topic, string oper_pass);
	ssize_t	addUser(string username, string fullname, string nickname, string hostname, string servername, Server* server);
};

#endif