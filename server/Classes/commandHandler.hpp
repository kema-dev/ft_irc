#ifndef _COMMAND_HANDLER_HPP_
 #define _COMMAND_HANDLER_HPP_

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "error.hpp"



enum Operator_commands {
    kick, ban, move, Last2 //Ajouter des commandes?
};


int command_check (std::string message);

#endif