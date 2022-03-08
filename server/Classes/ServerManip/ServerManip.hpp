#ifndef _SERVER_MANIP_HPP
 #define _SERVER_MANIP_HPP

#include "../Error/Error.hpp"
#include "../UserDB/UserDB.hpp"
#include <string>
#include <iostream>

void createUser(std::string input, UidPool pool);

#endif