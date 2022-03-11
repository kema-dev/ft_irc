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
User& createUser(std::string input, UidPool pool, int socket, string nickname);
string parseNickname(string input);
int check_password(string input, string server_password, int socket);

#endif