#ifndef _SERVER_MANIP_HPP
 #define _SERVER_MANIP_HPP

#include "../Error/Error.hpp"
#include "../UserDB/UserDB.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/event.h>

void init_kqueue(int socket, int &kq);
ssize_t createUser(std::string input, t_params *params, string nickname);
string parseNickname(string input);
int check_password(string input, Server *irc_serv, int socket);

#endif