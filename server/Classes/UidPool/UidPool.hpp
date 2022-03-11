#ifndef _UID_POOL_HPP
 #define _UID_POOL_HPP

#include <sys/types.h>
#include <iostream>
#include <limits.h>

using namespace std;

class PoolFull : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Too much users already got and uid");
		}
};

class UidPool {
	private:
	ssize_t	_count;
	string	_name;

	public:
	UidPool(string name);
	~UidPool() {};
	ssize_t	generate();
};

#endif