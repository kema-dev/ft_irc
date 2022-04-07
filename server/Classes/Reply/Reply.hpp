#ifndef _REPLY_HPP_
 #define _REPLY_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>

#include "../Server/Server.hpp"

using namespace std;

// string header_1(Server *server, ssize_t id, string rplyNb);
// string header_2(Server *server, ssize_t id, string rplyNb);

void reply_2(Server* serv, ssize_t uid, int rplyNb, string args, string args2);
void reply(Server* serv, ssize_t uid, int rplyNb, string args);

// Custom Reply
#define JOIN                    "4545"
#define PART                    "4546"
#define PRVMSG_U                "4547"
#define PRVMSG_C                "4548"
#define NICK                    "4549"
#define USER_R                  "4550"
#define TOPIC                   "4551"

// Reply
#define RPL_WELCOME				"001"
#define RPL_YOURHOST			"002"
#define RPL_CREATED				"003"
#define RPL_MYINFO				"004"
#define RPL_BOUNCE				"005"
#define RPL_UMODEIS				"221"
#define RPL_AWAY				"301"
#define RPL_UNAWAY				"305"
#define RPL_NOWAWAY				"306"
#define RPL_ENDOFWHO			"315"
#define RPL_LIST				"322" 
#define RPL_LISTEND				"323" 
#define RPL_NOTOPIC				"331" 
#define RPL_TOPIC				"332"
#define RPL_WHOREPLY			"352"
#define RPL_NAMEREPLY			"353"
#define RPL_ENDOFNAMES  		"366"
#define RPL_MOTD		  		"372"
#define RPL_MOTDSTART	  		"375"
#define RPL_ENDOFMOTD	  		"376"
#define RPL_YOUREOPER			"381"
#define RPL_CUSTOMCAP			"4242"
#define RPL_CUSTOMMODESUCCESS	"4243"
#define RPL_CUSTOMHELP      	"4244"
#define RPL_CUSTOMMOTD	     	"4245"

// Errors
#define ERR_NOSUCHNICK			"401"
#define ERR_NOSUCHCHANNEL		"403"
#define ERR_CANNOTSENDTOCHAN	"404"
#define ERR_TOOMANYCHANNELS		"405" 
#define ERR_TOOMANYTARGETS		"407" 
#define ERR_NORECIPIENT			"411"
#define ERR_NOTEXTTOSEND		"412"
#define ERR_UNKNOWNCOMMAND		"421"
#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_ERRONEUSNICKNAME	"432"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_USERNOTINCHANNEL    "441"
#define ERR_NOTONCHANNEL		"442"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_ALREADYREGISTERED	"462"
#define ERR_PASSWDMISMATCH  	"464"
#define ERR_CHANNELISFULL		"471"
#define	ERR_INVITEONLYCHAN		"473"
#define ERR_BANNEDFROMCHAN		"474"
#define ERR_BADCHANNELKEY		"475"
#define ERR_BADCHANMASK 		"476"
#define ERR_NOPRIVILEGES	    "481"
#define ERR_CHANOPRIVNEEDED     "482"
#define ERR_RESTRICTED			"484"
#define ERR_NOOPERHOST			"491"
#define ERR_UMODEUNKNOWNFLAG	"501"
#define ERR_USERSDONTMATCH		"502"
#define ERR_USERREALNAMEFORMAT	"4242"


#define JOIN_REPLY								4545
#define PART_REPLY								4546
#define PRVMSG_U_REPLY							4547
#define PRVMSG_C_REPLY							4548
#define NICK_REPLY								4549
#define USER_R_REPLY							4550
#define TOPIC_REPLY								4551
#define RPL_WELCOME_REPLY						1
#define RPL_YOURHOST_REPLY						2
#define RPL_CREATED_REPLY						3
#define RPL_MYINFO_REPLY						4
#define RPL_BOUNCE_REPLY						5
#define RPL_UMODEIS_REPLY						221
#define RPL_AWAY_REPLY							301
#define RPL_UNAWAY_REPLY						305
#define RPL_NOWAWAY_REPLY						306
#define RPL_ENDOFWHO_REPLY						315
#define RPL_LIST_REPLY							322 
#define RPL_LISTEND_REPLY						323 
#define RPL_NOTOPIC_REPLY						331 
#define RPL_TOPIC_REPLY							332
#define RPL_WHOREPLY_REPLY						352
#define RPL_NAMEREPLY_REPLY						353
#define RPL_ENDOFNAMES_REPLY  					366
#define RPL_MOTD_REPLY		  					372
#define RPL_MOTDSTART_REPLY	  					375
#define RPL_ENDOFMOTD_REPLY	  					376
#define RPL_YOUREOPER_REPLY						381
#define RPL_CUSTOMCAP_REPLY						4242
#define RPL_CUSTOMMODESUCCESS_REPLY				4243
#define RPL_CUSTOMHELP_REPLY	  				4244
#define RPL_CUSTOMMOTD_REPLY		 			4245
#define ERR_NOSUCHNICK_REPLY					401
#define ERR_NOSUCHCHANNEL_REPLY					403
#define ERR_CANNOTSENDTOCHAN_REPLY				404
#define ERR_TOOMANYCHANNELS_REPLY				405 
#define ERR_TOOMANYTARGETS_REPLY				407 
#define ERR_NORECIPIENT_REPLY					411
#define ERR_NOTEXTTOSEND_REPLY					412
#define ERR_UNKNOWNCOMMAND_REPLY				421
#define ERR_NONICKNAMEGIVEN_REPLY				431
#define ERR_ERRONEUSNICKNAME_REPLY				432
#define ERR_NICKNAMEINUSE_REPLY					433
#define ERR_USERNOTINCHANNEL_REPLY				441
#define ERR_NOTONCHANNEL_REPLY					442
#define ERR_NEEDMOREPARAMS_REPLY				461
#define ERR_ALREADYREGISTERED_REPLY				462
#define ERR_PASSWDMISMATCH_REPLY  				464
#define ERR_CHANNELISFULL_REPLY					471
#define	ERR_INVITEONLYCHAN_REPLY				473
#define ERR_BANNEDFROMCHAN_REPLY				474
#define ERR_BADCHANNELKEY_REPLY					475
#define ERR_BADCHANMASK_REPLY 					476
#define ERR_NOPRIVILEGES_REPLY					481
#define ERR_CHANOPRIVNEEDED_REPLY				482
#define ERR_RESTRICTED_REPLY					484
#define ERR_NOOPERHOST_REPLY					491
#define ERR_UMODEUNKNOWNFLAG_REPLY				501
#define ERR_USERSDONTMATCH_REPLY				502
#define ERR_USERREALNAMEFORMAT_REPLY			4242
#endif
