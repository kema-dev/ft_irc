#include "../Classes/uid_pool.hpp"

using namespace std;

UidPool::UidPool() {
	_count = 1;
}

size_t	UidPool::generate() {
	if (_count >= ULONG_MAX) {
		throw (PoolFull());
		return (0);
	}
	_count += 1;
	return (_count - 1);
}
