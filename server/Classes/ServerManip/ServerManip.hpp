#ifndef _SERVER_MANIP_HPP
 #define _SERVER_MANIP_HPP

#include "../Error/Error.hpp"
#include "../UserDB/UserDB.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

User& createUser(std::string input, UidPool pool, int socket);

#endif