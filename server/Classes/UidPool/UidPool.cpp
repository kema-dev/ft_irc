#include "UidPool.hpp"

using namespace std;

UidPool::UidPool(string name) {
	_name = name;
	_count = 0;
}

ssize_t	UidPool::generate() {
	if (_count >= SSIZE_MAX) {
		throw (PoolFull());
		return (-1);
	}
	ssize_t ret = _count;
	_count += 1;
	return (ret);
}
