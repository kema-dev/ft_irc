#include "Reply.hpp"

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
        case 331:
            reply = header_1(params->irc_serv, params->user_id, RPL_NOTOPIC);
            reply += params->irc_serv->chanDB->search(args)->getName() + " :No topic is set.\r\n";
            break;
        case 332:
            reply = header_1(params->irc_serv, params->user_id, RPL_TOPIC);
            reply += params->irc_serv->chanDB->search(args)->getName() + " :" + params->irc_serv->chanDB->search(args)->getTopic() + "\r\n";
            break;
        case 353:
            reply = header_1(params->irc_serv, params->user_id, RPL_NAMEREPLY);
            reply += (" = " + params->irc_serv->userDB->search(params->user_id)->getUserName() + " :");
			vector<string> tmp = params->irc_serv->chanDB->search(args)->getNickLst();
			vector<string>::iterator it = tmp.begin();
			vector<string>::iterator ite = tmp.end();
			while (it != ite)
			{
				if (params->irc_serv->chanDB->search(args)->isOper(*it) == true)
					reply += "@";
				reply += ((*it) + " ");
				it++;
			}
			reply += "\r\n";
            break;
        case 366:
            reply = header_1(params->irc_serv, params->user_id, RPL_ENDOFNAMES);
            reply += params->irc_serv->chanDB->search(args)->getName() + " :End of NAMES list\r\n";
            break;
        case 4545:
            reply = header_2(params->irc_serv, params->user_id, "");
            reply += "JOIN " + params->irc_serv->chanDB->search(args)->getName() + "\r\n";
            break;
        default:
            break;
    }
    send(params->client_socket, reply.c_str(), strlen(reply.c_str()), 0);
    return;
}
