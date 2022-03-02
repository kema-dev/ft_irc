#ifndef _COMMAND_HANDLER_HPP_
 #define _COMMAND_HANDLER_HPP_

#include "../Error/Error.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

enum Operator_commands {
    kick, ban, move, Last2 //Ajouter des commandes?
};

int command_check(std::string message, int fd);

#endif