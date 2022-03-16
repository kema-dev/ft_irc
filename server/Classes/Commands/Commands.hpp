#ifndef _COMMAND_HPP_
 #define _COMMAND_HPP_

#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Reply/Reply.hpp"
#include "../CommandHandler/CommandHandler.hpp"

void Join(t_params *params, string channel_s);
void Part(t_params *params, string channel_s);

#endif