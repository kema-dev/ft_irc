#ifndef _COMMAND_HPP_
 #define _COMMAND_HPP_

#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Reply/Reply.hpp"
#include "../CommandHandler/CommandHandler.hpp"

void welcome_client(t_params *params, string channel_s);
void Join(t_params *params, string channel_s);
void Part(t_params *params, string channel_s, string msg);
void Nick(t_params *params, string args);
void User(t_params * params, string args);
void Topic(t_params *params, string args, string topic);
void PrivateMessage(t_params *params, string args, string message);

#endif