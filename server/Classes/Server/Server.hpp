#ifndef _SERVER_HPP
 #define _SERVER_HPP

#include <sys/types.h>
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sstream>
#include <string>

class ChannelDB;
class Server;

typedef struct s_params
{
    int     client_socket;
    Server  *irc_serv;
    ssize_t user_id;
}   t_params;

#include "../Commands/Commands.hpp"
#include "../UidPool/UidPool.hpp"
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"
#include "../Error/Error.hpp"
#include "../ServerManip/ServerManip.hpp"


using namespace std;

typedef struct s_KDescriptor 
{
    Server  *server;
    User    *user;
    bool    connected;
}   t_KDescriptor;

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
    int         _socket;

	public:
    vector<t_KDescriptor*> _descriptors;
    bool        _running;
	string		name;
	UidPool*	pool;
	ChannelDB*	chanDB;
	UserDB*		userDB;

	public:
	Server(string dname, string pass, int socket);
	~Server();

    void    start( void );
    int     getSocket( void );
    void    setSocket( int socket );
    string  getHash();
    void    acceptConnection(t_KDescriptor *desc,int socket);
	void	addChan(string name, string pass, string topic);
    void    handleConnection(t_KDescriptor *desc, int socket);
    string  readSocket(int socket);
	// void	addChan(string name, string pass, string topic, string oper_pass);
    void    addVoidUser( void );
	ssize_t	addUser(string username, string fullname, string nickname, string hostname, string servername, Server* server, int socket);
	void	setPasswd(string pass);
	// bool	checkOperPasswd(string pass);
	// bool	setOperPasswd(string oper_pass);
	// bool	addOper(User& usr);
	// bool	removeOper(User& usr);
};

#endif