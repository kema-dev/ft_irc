#ifndef _USERDB_HPP
 #define _USERDB_HPP
#include "../User/User.hpp"
#include "../Crypto/Crypto.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

class UserDB {
	private:
	vector<User>	_db;

	public:
	UserDB() {};
	~UserDB() {};

	void	add(User usr);
	User*	search(User usr);
	User*	search(ssize_t id);
};

#endif