#include "Commands.hpp"

void Join(t_params *params, string channel_s)
{
    try {
        params->irc_serv->userDB->search(params->user_id)->tryJoinChannel(channel_s, "", "", "", params->irc_serv);
    }
    catch (exception& e) {
        string str = itos(params->user_id);
        logError(string("Joining channel " + channel_s + " on server"), str, e.what());
    }
    reply(params, JOIN, channel_s);
    if (params->irc_serv->chanDB->search(channel_s)->getTopic().empty())
        reply(params, RPL_NOTOPIC, channel_s);
    else
        reply(params, RPL_TOPIC, channel_s);
    
    send(params->client_socket, ":127.0.0.1 353 dOD = #ratio :@dOD\r\n", strlen(":127.0.0.1 353 dOD = #ratio :@dOD\r\n"), 0);
    send(params->client_socket, ":127.0.0.1 366 dOD #ratio :End of NAMES list\r\n", strlen(":127.0.0.1 366 dOD #ratio :End of NAMES list\r\n"), 0);
    return;
}

void Part(t_params *params, string channel_s)
{
    (void)params;
    (void)channel_s;

    return;
}