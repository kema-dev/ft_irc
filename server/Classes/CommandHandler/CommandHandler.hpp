#ifndef _COMMAND_HANDLER_HPP_
 #define _COMMAND_HANDLER_HPP_

#include "../Error/Error.hpp"
#include "../Command/Command.hpp"
#include "../Channel/Channel.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

// ! SEND ONLY "<< overloaded" type !
template<typename T>
string	itos(T nb) {
    return static_cast<ostringstream*>(&(ostringstream() << nb))->str();
}

int command_check(std::string message, t_params *params);

#endif