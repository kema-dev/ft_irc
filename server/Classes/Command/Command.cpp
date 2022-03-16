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

void reply(t_params *params, string replyNb, string args)
{
    string reply;

    switch(atoi(replyNb.c_str()))
    {
        case 332:
            reply = header_1(params->irc_serv, params->user_id, RPL_TOPIC);
            reply = reply + params->irc_serv->chanDB->search(args)->getName() + " :" + params->irc_serv->chanDB->search(args)->getTopic() + "\r\n";
            break;
        case 4545:
            reply = header_2(params->irc_serv, params->user_id, "");
            reply = reply + "JOIN " + params->irc_serv->chanDB->search(args)->getName() + "\r\n";
            break;
        default:
            break;
    }
    send(params->client_socket, reply.c_str(), strlen(reply.c_str()), 0);
    return;
}
