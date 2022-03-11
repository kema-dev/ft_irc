#ifndef _SERVER_HPP
 #define _SERVER_HPP

#include "../UidPool/UidPool.hpp"
class Channel;
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"

#include <sys/types.h>
#include <iostream>

using namespace std;

class NoMem : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Function 'new' failed");
		}
};

class DeleteFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Function 'delete' failed");
		}
};

class Server {
	private:
	string		_hash;

	public:
	string		name;
	UidPool		pool;
	ChannelDB	chanDB;
	UserDB		userDB;

	public:
	Server(string dname, string pass);
	~Server();
};

#endif