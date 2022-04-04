#ifndef _UTILS_HPP
#define _UTILS_HPP

using namespace std;

#include <sstream>

// ! SEND ONLY "<< overloaded" type !
template <typename T>
string itos(T nb) {
	return static_cast<ostringstream*>(&(ostringstream() << nb))->str();
}

#endif