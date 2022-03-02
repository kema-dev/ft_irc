#include "UidPool.hpp"

using namespace std;

UidPool::UidPool() {
	_count = 0;
}

ssize_t	UidPool::generate() {
	if (_count >= ULONG_MAX) {
		throw (PoolFull());
		return (-1);
	}
	_count += 1;
	return (_count - 1);
}
