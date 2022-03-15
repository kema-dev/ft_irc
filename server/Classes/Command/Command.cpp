#include "Command.hpp"

string header_1(Server *server, ssize_t id, string rplyNb)
{
    string reply;

    reply = ":" + server->userDB->search(id)->getHostName() + " " + rplyNb + " " + server->userDB->search(id)->getNickName() + " :";
    return(reply);
}

string header_2(Server *server, ssize_t id, string rplyNb)
{
    string reply;
    if (rplyNb.empty())
        reply = ":" + server->userDB->search(id)->getNickName() + "!" + server->userDB->search(id)->getUserName() + "@" + server->userDB->search(id)->getHostName() + " ";
    else
        reply = ":" + server->userDB->search(id)->getNickName() + "!" + server->userDB->search(id)->getUserName() + "@" + server->userDB->search(id)->getHostName() + " " + rplyNb + " : ";
    return(reply);
}
