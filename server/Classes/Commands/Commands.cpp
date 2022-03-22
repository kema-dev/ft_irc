#include "Commands.hpp"

void welcome_client(t_params * params, string channel_s)
{
    reply(params, RPL_WELCOME, channel_s);
    reply(params, RPL_YOURHOST, channel_s);
    reply(params, RPL_CREATED, channel_s);
    reply(params, RPL_MYINFO, channel_s);
}

void Join(t_params *params, string channel_s)
{
    try {
        params->irc_serv->userDB->search(params->user_id)->tryJoinChannel(channel_s, "", "", "", params->irc_serv);
    }
    catch (exception& e) {
        string str = itos(params->user_id);
        logError(string("Joining channel " + channel_s + " on server"), str, e.what());
        return;
    }
    reply(params, JOIN, channel_s);
    if (params->irc_serv->chanDB->search(channel_s)->getTopic().empty() == true)
        reply(params, RPL_NOTOPIC, channel_s);
    else
        reply(params, RPL_TOPIC, channel_s);
    reply(params, RPL_NAMEREPLY, channel_s);
    reply(params, RPL_ENDOFNAMES, channel_s);
    return;
}

void Part(t_params *params, string channel_s, string msg)
{
    try {
        params->irc_serv->chanDB->search(channel_s)->userLeave(*(params->irc_serv->userDB->search(params->user_id)));
    }
    catch (exception& e) {
        string str = itos(params->user_id);
        logError(string("Leaving channel " + channel_s + " on server"), str, e.what());
        return;
    }
    reply_2(params, PART, channel_s, msg);
    return;
}

void Nick(t_params * params, string args)
{
    try {
        if (params->irc_serv->userDB->search(args) != nullptr)
            
    }
    catch(exception& e) {
        // NOTHING TO DO KEKW
    }
    try {
        params->irc_serv->userDB->search(params->user_id)->setNickName(args);
    }
    catch(exception& e) {
        logError(string("Set Nickname"), args, e.what());
        return ;
    }
    reply(params, NICK, args);
}

void User(t_params * params, string args)
{
    try {
        params->irc_serv->userDB->search(params->user_id)->setNickName(args);
    }
    catch(exception& e) {
        logError(string("Set Usename"), args, e.what());
        return;
    }
}

void Topic(t_params *params, string args, string topic)
{
    string channel_s = args;

    try {
        if (params->irc_serv->chanDB->search(channel_s)->isLog(*(params->irc_serv->userDB->search(params->user_id))) != CONNECTED)
            throw isNotLogged();  
    }
    catch(exception& e) {
        logError(string("Not logged in"), channel_s, e.what());
        return ;
    }
    try{
        params->irc_serv->chanDB->search(channel_s)->setTopic(topic);
    }
    catch(exception& e){
        logError(string("Searching channel"), channel_s, e.what());
        return ;
    }
    cerr << "'" << topic << "'" << endl;
    if (topic.empty())
    {
        if (params->irc_serv->chanDB->search(args)->getTopic().empty())
            topic = "No topic is set.";
        else
            topic = params->irc_serv->chanDB->search(args)->getTopic();
    }
    reply_2(params, TOPIC, args, topic);
}

void PrivateMessage(t_params *params, string args, string message)
{
    bool pass = true;
    try {
        params->irc_serv->chanDB->search(args);
    }
    catch(exception& e) {
        pass = false;
        logError(string("Searching channel"), args, e.what());
    }
    if (pass == true){
        try {
            if (params->irc_serv->chanDB->search(args)->isLog(*(params->irc_serv->userDB->search(params->user_id))) != CONNECTED)
            throw isNotLogged();  
        }
        catch(exception& e) {
            logError(string("Not logged in"), args, e.what());
            return ;
        }
        reply_2(params, PRVMSG_C, args, message);
        return;
    }
    else {
        pass = true;
        try {
            params->irc_serv->userDB->search(args);
        }
        catch(exception& e) {
            pass = false;
            logError(string("Searching User"), args, e.what());
        }
        if (pass == true){
            reply_2(params, PRVMSG_U, args, message);
            return;
        }
    }
    return;
}
