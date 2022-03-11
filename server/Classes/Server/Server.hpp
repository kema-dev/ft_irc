#ifndef _SERVER_HPP
 #define _SERVER_HPP

class Server;
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"
#include "../Crypto/Crypto.hpp"
#include "../UidPool/UidPool.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

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
};

#endif