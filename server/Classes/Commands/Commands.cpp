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
        params->irc_serv->userDB->search(params->user_id)->tryJoinChannel(channel_s, "", "", params->irc_serv);
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
    // try {
    //     if (params->irc_serv->userDB->search(args) != nullptr)
            
    // }
    // catch(exception& e) {
    //     // NOTHING TO DO KEKW
    // }
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

// FIXME check if more than 2 params
void	Oper(string message, t_params* params) {
	string password;
	try {
		parse_oper(message, &password);
	}
	catch (NeedMoreParams& e) {
		logError(string("Set new operator on server " + params->irc_serv->name), "Need more params", e.what());
		// TODO send ERR_NEEDMOREPARAMS
		return ;
	}
	try {
		(*params->irc_serv->userDB->search(params->user_id)).becomeOper(*params->irc_serv, password);
	}
	catch (BadPasswd& e) {
		logError(string("Set new operator on server " + params->irc_serv->name), "Bad password", e.what());
		// TODO send ERR_PASSWDMISMATCH
		return ;
	}
	catch (exception& e) {
		logError(string("Set new operator on server " + params->irc_serv->name), "Default error", e.what());
		// TODO send ERR_PASSWDMISMATCH (as default error, should never catch except here)
		return ;
	}
	// TODO send RPL_YOUREOPER
	// TODO send MODE
}


void	Kick(string message, t_params* params) {
	string	kicker;
	try {
		kicker = params->irc_serv->userDB->search(params->user_id)->getNickName();
	}
	catch (exception& e) {
		logError(string("Kick user from channel"), "Unknown kicker", e.what());
	}
	string	kickMsg = kicker;
	string	chan;
	vector<string>	user;
	try {
		parse_kick(message, &chan, &user, &kickMsg, params->irc_serv);
	}
	catch (exception& e) {
		logError(string("Kick user from channel " + chan), *(user.begin()), e.what());
		// TODO send ERR_NEEDMOREPARAMS
	}
	try {
		params->irc_serv->chanDB->search(chan);
	}
	catch (exception& e) {
		logError(string("Kick user from channel " + chan), *(user.begin()), e.what());
		// TODO send ERR_NOSUCHCHANNEL
	}
	try {
		params->irc_serv->chanDB->search(chan)->isLog(*(params->irc_serv->userDB->search(kicker)));
	}
	catch (exception& e) {
		logError(string("Kick user from channel " + chan), *(user.begin()), e.what());
		// TODO send ERR_NOTONCHANNEL
	}
	string	usr = *(user.begin());
	// string	usr = "";
	try {
		while (!usr.empty() && usr != ":") {
			try {
				params->irc_serv->chanDB->search(chan)->userLeave(*(params->irc_serv->userDB->search(usr)));
			}
			catch (exception& e) {
				logError(string("Kick user from channel " + chan), *(user.begin()), e.what());
				// TODO send ERR_USERNOTINCHANNEL with kickMsg
			}
			log(string(LIGHT_MAGENTA) +  string("User ") +  string(RED) +  usr +  string(LIGHT_BLUE) +  string(" has been kicked out from ") + string(LIGHT_MAGENTA) + string("channel ") + string(RED) + chan + string(LIGHT_BLUE) + " by " +  string(RED) + kicker + string(DEFAULT));
			user.erase(user.begin());
			usr = *(user.begin());
		}
	}
	catch (exception& e) {
		logError(string("Kick user from channel " + chan), *(user.begin()), e.what());
	}
}
