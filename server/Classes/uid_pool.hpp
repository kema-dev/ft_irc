#ifndef _UID_POOL_HPP
 #define _UID_POOL_HPP

#include <sys/types.h>
#include <iostream>
#include <limits.h>

using namespace std;

class PoolFull : public exception
{
	public:
		virtual const string	info() const throw()
		{
			return ("Too much users already got and uid");
		}
};

class UidPool {
	private:
	size_t	_count;

	public:
	UidPool();
	~UidPool() {};
	size_t	generate();
};

#endif