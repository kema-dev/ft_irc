#ifndef _SERVER_MANIP_HPP
 #define _SERVER_MANIP_HPP

#include "../Error/Error.hpp"
#include "../Server/Server.hpp"
#include "../UserDB/UserDB.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/event.h>

class ServerManip {
    private:
        string _input;
        string _buf;
    public:
        ServerManip();
        ~ServerManip();

        void    setInput(string input);
        void    setBuf(string buf);

        string  getInput(void);
        string  getBuf(void);

        int     check_password(string input, Server *irc_serv, int socket);
        size_t  ft_find(string input);
        ssize_t createUser(string input, t_params *params, string nickname, t_KDescriptor *desc);
        string  parseNickname(string input);
        string  parseInput(string input);
};


#endif