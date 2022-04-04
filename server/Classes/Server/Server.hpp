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

#include "../Command/Command/Command.hpp"
#include "../UidPool/UidPool.hpp"
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"
#include "../Error/Error.hpp"
#include "../Crypto/Crypto.hpp"

typedef struct s_KDescriptor 
{
    Server  *server;
    User    *user;
}   t_KDescriptor;

#include "../ServerManip/ServerManip.hpp"


using namespace std;


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
    void    handleConnection(t_KDescriptor *desc);
    string  readSocket(int socket);
	// void	addChan(string name, string pass, string topic, string oper_pass);
    void    addVoidUser( User* user );
	ssize_t	addUser(t_KDescriptor *desc, string username, string fullname, string hostname, string servername);
	void	setPasswd(string pass);
	// bool	checkOperPasswd(string pass);
	// bool	setOperPasswd(string oper_pass);
	// bool	addOper(User& usr);
	// bool	removeOper(User& usr);
};

#endif