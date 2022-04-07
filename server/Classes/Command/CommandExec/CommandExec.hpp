#ifndef _COMMANDEXEC_HPP
 #define _COMMANDEXEC_HPP

class Send;
#include "../Command/Command.hpp"
#include "../../Send/Send.hpp"
#include "../../User/User.hpp"
#include "../../Reply/Reply.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

// ? "1 Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_WELCOME_FORMAT "1 Welcome to the Internet Relay Network %s!%s@%s\r\n"
// ? "2 Your host is <servername>, running version <ver>"
#define RPL_YOURHOST_FORMAT "2 Your host is %s, running version %s\r\n"
// ? "3 This server was created <date>"
#define RPL_CREATED_FORMAT "3 This server was created %s\r\n"
// ? "4 <servername> <version> <available user modes> <available channel modes>"
#define RPL_MYINFO_FORMAT "4 %s %s %s %s\r\n"
// ? "5 Try server <server name>, port <port number>"
#define RPL_BOUNCE_FORMAT "5 Try server %s, port %s\r\n"
// ? "200 Link <version & debug level> <destination> <next server> V<protocol version> <link uptime in seconds> <backstream sendq> <upstream sendq>"
#define RPL_TRACELINK_FORMAT "200 Link %s %s %s V%s %s %s %s\r\n"
// ? "201 Try. <class> <server>"
#define RPL_TRACECONNECTING_FORMAT "201 Try. %s %s\r\n"
// ? "202 H.S. <class> <server>"
#define RPL_TRACEHANDSHAKE_FORMAT "202 H.S. %s %s\r\n"
// ? "203 ???? <class> [<client IP address in dot form>]"
#define RPL_TRACEUNKNOWN_FORMAT "203 ???? %s [%s]\r\n"
// ? "204 Oper <class> <nick>"
#define RPL_TRACEOPERATOR_FORMAT "204 Oper %s %s\r\n"
// ? "205 User <class> <nick>"
#define RPL_TRACEUSER_FORMAT "205 User %s %s\r\n"
// ? "206 Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server> V<protocol version>"
#define RPL_TRACESERVER_FORMAT "206 Serv %s %sS %sC %s %s@%s V%s\r\n"
// ? "207 Service <class> <name> <type> <active type>"
#define RPL_TRACESERVICE_FORMAT "207 Service %s %s %s %s\r\n"
// ? "208 <newtype> 0 <client name>"
#define RPL_TRACENEWTYPE_FORMAT "208 %s 0 %s\r\n"
// ? "209 Class <class> <count>"
#define RPL_TRACECLASS_FORMAT "209 Class %s %s\r\n"
// ? "210 Unused."
#define RPL_TRACERECONNECT_FORMAT "210 Unused.\r\n"
// ? "211 <linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>"
#define RPL_STATSLINKINFO_FORMAT "211 %s %s %s %s %s %s %s\r\n"
// ? "212 <command> <count> <byte count> <remote count>"
#define RPL_STATSCOMMANDS_FORMAT "212 %s %s %s %s\r\n"
// ? "219 <stats letter> :End of STATS report"
#define RPL_ENDOFSTATS_FORMAT "219 %s :End of STATS report\r\n"
// ? "221 <user mode string>"
#define RPL_UMODEIS_FORMAT "221 %s\r\n"
// ? "234 <name> <server> <mask> <type> <hopcount> <info>"
#define RPL_SERVLIST_FORMAT "234 %s %s %s %s %s %s\r\n"
// ? "235 <mask> <type> :End of service listing"
#define RPL_SERVLISTEND_FORMAT "235 %s %s :End of service listing\r\n"
// ? "242 :Server Up %d days %d:%02d:%02d"
#define RPL_STATSUPTIME_FORMAT "242 :Server Up %d days %d:%02d:%02d\r\n"
// ? "243 O <hostmask> * <name>"
#define RPL_STATSOLINE_FORMAT "243 O %s * %s\r\n"
// ? "251 :There are <integer> users and <integer> services on <integer> servers"
#define RPL_LUSERCLIENT_FORMAT "251 :There are %s users and %s services on %s servers\r\n"
// ? "252 <integer> :operator(s) online"
#define RPL_LUSEROP_FORMAT "252 %s :operator(s) online\r\n"
// ? "253 <integer> :unknown connection(s)"
#define RPL_LUSERUNKNOWN_FORMAT "253 %s :unknown connection(s)\r\n"
// ? "254 <integer> :channels formed"
#define RPL_LUSERCHANNELS_FORMAT "254 %s :channels formed\r\n"
// ? "255 :I have <integer> clients and <integer> servers"
#define RPL_LUSERME_FORMAT "255 :I have %s clients and %s servers\r\n"
// ? "256 <server> :Administrative info"
#define RPL_ADMINME_FORMAT "256 %s :Administrative info\r\n"
// ? "257 :<admin info>"
#define RPL_ADMINLOC1_FORMAT "257 :%s\r\n"
// ? "258 :<admin info>"
#define RPL_ADMINLOC2_FORMAT "258 :%s\r\n"
// ? "259 :<admin info>"
#define RPL_ADMINEMAIL_FORMAT "259 :%s\r\n"
// ? "261 File <logfile> <debug level>"
#define RPL_TRACELOG_FORMAT "261 File %s %s\r\n"
// ? "262 <server name> <version & debug level> :End of TRACE"
#define RPL_TRACEEND_FORMAT "262 %s %s :End of TRACE\r\n"
// ? "263 <command> :Please wait a while and try again."
#define RPL_TRYAGAIN_FORMAT "263 %s :Please wait a while and try again.\r\n"
// ? "301 <nick> :<away message>"
#define RPL_AWAY_FORMAT "301 %s :%s\r\n"
// ? "302 :*1<reply> *( " " <reply> )"
#define RPL_USERHOST_FORMAT "302 :*1%s *( " " %s )\r\n"
// ? "303 :*1<nick> *( " " <nick> )"
#define RPL_ISON_FORMAT "303 :*1%s *( " " %s )\r\n"
// ? "305 :You are no longer marked as being away"
#define RPL_UNAWAY_FORMAT "305 :You are no longer marked as being away\r\n"
// ? "306 :You have been marked as being away"
#define RPL_NOWAWAY_FORMAT "306 :You have been marked as being away\r\n"
// ? "311 <nick> <user> <host> * :<real name>"
#define RPL_WHOISUSER_FORMAT "311 %s %s %s * :%s\r\n"
// ? "312 <nick> <server> :<server info>"
#define RPL_WHOISSERVER_FORMAT "312 %s %s :%s\r\n"
// ? "313 <nick> :is an IRC operator"
#define RPL_WHOISOPERATOR_FORMAT "313 %s :is an IRC operator\r\n"
// ? "314 <nick> <user> <host> * :<real name>"
#define RPL_WHOWASUSER_FORMAT "314 %s %s %s * :%s\r\n"
// ? "315 <name> :End of WHO list"
#define RPL_ENDOFWHO_FORMAT "315 %s :End of WHO list\r\n"
// ? "317 <nick> <integer> :seconds idle"
#define RPL_WHOISIDLE_FORMAT "317 %s %s :seconds idle\r\n"
// ? "318 <nick> :End of WHOIS list"
#define RPL_ENDOFWHOIS_FORMAT "318 %s :End of WHOIS list\r\n"
// ? "319 <nick> :*( ( \"@\" / \"+\" ) <channel> " " )"
#define RPL_WHOISCHANNELS_FORMAT "319 %s :*( ( \"@\" / \"+\" ) %s " " )\r\n"
// ? "321 Obsolete."
#define RPL_LISTSTART_FORMAT "321 Obsolete.\r\n"
// ? "322 <channel> <# visible> :<topic>"
#define RPL_LIST_FORMAT "322 %s %s :%s\r\n"
// ? "323 :End of LIST"
#define RPL_LISTEND_FORMAT "323 :End of LIST\r\n"
// ? "324 <channel> <mode> <mode params>"
#define RPL_CHANNELMODEIS_FORMAT "324 %s %s %s\r\n"
// ? "325 <channel> <nickname>"
#define RPL_UNIQOPIS_FORMAT "325 %s %s\r\n"
// ? "331 <channel> :No topic is set"
#define RPL_NOTOPIC_FORMAT "331 %s :No topic is set\r\n"
// ? "332 <channel> :<topic>"
#define RPL_TOPIC_FORMAT "332 %s :%s\r\n"
// ? "341 <channel> <nick>"
#define RPL_INVITING_FORMAT "341 %s %s\r\n"
// ? "342 <user> :Summoning user to IRC"
#define RPL_SUMMONING_FORMAT "342 %s :Summoning user to IRC\r\n"
// ? "346 <channel> <invitemask>"
#define RPL_INVITELIST_FORMAT "346 %s %s\r\n"
// ? "347 <channel> :End of channel invite list"
#define RPL_ENDOFINVITELIST_FORMAT "347 %s :End of channel invite list\r\n"
// ? "348 <channel> <exceptionmask>"
#define RPL_EXCEPTLIST_FORMAT "348 %s %s\r\n"
// ? "349 <channel> :End of channel exception list"
#define RPL_ENDOFEXCEPTLIST_FORMAT "349 %s :End of channel exception list\r\n"
// ? "351 <version>.<debuglevel> <server> :<comments>"
#define RPL_VERSION_FORMAT "351 %s.%s %s :%s\r\n"
// ? "352 <channel> <user> <host> <server> <nick> ( \"H\" / \"G\" > [\"*\"] [ ( \"@\" / \"+\" ) ] :<hopcount> <real name>"
#define RPL_WHOREPLY_FORMAT "352 %s %s %s %s %s ( \"H\" / \"G\" > [\"*\"] [ ( \"@\" / \"+\" ) ] :%s %s\r\n"
// ? "353 ( \"=\" / \"*\" / \"@\" ) <channel> :[ \"@\" / \"+\" ] <nick> *( \" \" [ \"@\" / \"+\" ] <nick> )"
#define RPL_NAMREPLY_FORMAT "353 ( \"=\" / \"*\" / \"@\" ) %s :[ \"@\" / \"+\" ] %s *( \" \" [ \"@\" / \"+\" ] %s )\r\n"
// ? "364 <mask> <server> :<hopcount> <server info>"
#define RPL_LINKS_FORMAT "364 %s %s :%s %s\r\n"
// ? "365 <mask> :End of LINKS list"
#define RPL_ENDOFLINKS_FORMAT "365 %s :End of LINKS list\r\n"
// ? "366 <channel> :End of NAMES list"
#define RPL_ENDOFNAMES_FORMAT "366 %s :End of NAMES list\r\n"
// ? "367 <channel> <banmask>"
#define RPL_BANLIST_FORMAT "367 %s %s\r\n"
// ? "368 <channel> :End of channel ban list"
#define RPL_ENDOFBANLIST_FORMAT "368 %s :End of channel ban list\r\n"
// ? "369 <nick> :End of WHOWAS"
#define RPL_ENDOFWHOWAS_FORMAT "369 %s :End of WHOWAS\r\n"
// ? "371 :<string>"
#define RPL_INFO_FORMAT "371 :%s\r\n"
// ? "372 :- <text>"
#define RPL_MOTD_FORMAT "372 :- %s\r\n"
// ? "374 :End of INFO list"
#define RPL_ENDOFINFO_FORMAT "374 :End of INFO list\r\n"
// ? "375 :- <server> Message of the day - "
#define RPL_MOTDSTART_FORMAT "375 :- %s Message of the day - \r\n"
// ? "376 :End of MOTD command"
#define RPL_ENDOFMOTD_FORMAT "376 :End of MOTD command\r\n"
// ? "381 :You are now an IRC operator"
#define RPL_YOUREOPER_FORMAT "381 :You are now an IRC operator\r\n"
// ? "382 <config file> :Rehashing"
#define RPL_REHASHING_FORMAT "382 %s :Rehashing\r\n"
// ? "383 You are service <servicename>"
#define RPL_YOURESERVICE_FORMAT "383 You are service %s\r\n"
// ? "391 <server> :<string showing server's local time>"
#define RPL_TIME_FORMAT "391 %s :%s\r\n"
// ? "392 :UserID Terminal Host"
#define RPL_USERSSTART_FORMAT "392 :UserID Terminal Host\r\n"
// ? "393 :<username> <ttyline> <hostname>"
#define RPL_USERS_FORMAT "393 :%s %s %s\r\n"
// ? "394 :End of users"
#define RPL_ENDOFUSERS_FORMAT "394 :End of users\r\n"
// ? "395 :Nobody logged in"
#define RPL_NOUSERS_FORMAT "395 :Nobody logged in\r\n"


// ? "401 <nickname> :No such nick/channel"
#define ERR_NOSUCHNICK_FORMAT "401 %s :No such nick/channel\r\n"
// ? "402 <server name> :No such server"
#define ERR_NOSUCHSERVER_FORMAT "402 %s :No such server\r\n"
// ? "403 <channel name> :No such channel"
#define ERR_NOSUCHCHANNEL_FORMAT "403 %s :No such channel\r\n"
// ? "404 <channel name> :Cannot send to channel"
#define ERR_CANNOTSENDTOCHAN_FORMAT "404 %s :Cannot send to channel\r\n"
// ? "405 <channel name> :You have joined too many channels"
#define ERR_TOOMANYCHANNELS_FORMAT "405 %s :You have joined too many channels\r\n"
// ? "406 <nickname> :There was no such nickname"
#define ERR_WASNOSUCHNICK_FORMAT "406 %s :There was no such nickname\r\n"
// ? "407 <target> :<error code> recipients. <abort message>"
#define ERR_TOOMANYTARGETS_FORMAT "407 %s :%s recipients. %s\r\n"
// ? "408 <service name> :No such service"
#define ERR_NOSUCHSERVICE_FORMAT "408 %s :No such service\r\n"
// ? "409 :No origin specified"
#define ERR_NOORIGIN_FORMAT "409 :No origin specified\r\n"
// ? "411 :No recipient given (<command>)"
#define ERR_NORECIPIENT_FORMAT "411 :No recipient given (%s)\r\n"
// ? "412 :No text to send"
#define ERR_NOTEXTTOSEND_FORMAT "412 :No text to send\r\n"
// ? "413 <mask> :No toplevel domain specified"
#define ERR_NOTOPLEVEL_FORMAT "413 %s :No toplevel domain specified\r\n"
// ? "414 <mask> :Wildcard in toplevel domain"
#define ERR_WILDTOPLEVEL_FORMAT "414 %s :Wildcard in toplevel domain\r\n"
// ? "415 <mask> :Bad Server/host mask"
#define ERR_BADMASK_FORMAT "415 %s :Bad Server/host mask\r\n"
// ? "421 <command> :Unknown command"
#define ERR_UNKNOWNCOMMAND_FORMAT "421 %s :Unknown command\r\n"
// ? "422 :MOTD File is missing"
#define ERR_NOMOTD_FORMAT "422 :MOTD File is missing\r\n"
// ? "423 <server> :No administrative info available"
#define ERR_NOADMININFO_FORMAT "423 %s :No administrative info available\r\n"
// ? "424 :File error doing <file op> on <file>"
#define ERR_FILEERROR_FORMAT "424 :File error doing %s on %s\r\n"
// ? "431 :No nickname given"
#define ERR_NONICKNAMEGIVEN_FORMAT "431 :No nickname given\r\n"
// ? "432 <nick> :Erroneous nickname"
#define ERR_ERRONEUSNICKNAME_FORMAT "432 %s :Erroneous nickname\r\n"
// ? "433 <nick> :Nickname is already in use"
#define ERR_NICKNAMEINUSE_FORMAT "433 %s :Nickname is already in use\r\n"
// ? "436 <nick> :Nickname collision KILL from <user>@<host>"
#define ERR_NICKCOLLISION_FORMAT "436 %s :Nickname collision KILL from %s@%s\r\n"
// ? "437 <nick/channel> :Nick/channel is temporarily unavailable"
#define ERR_UNAVAILRESOURCE_FORMAT "437 %s :Nick/channel is temporarily unavailable\r\n"
// ? "441 <nick> <channel> :They aren't on that channel"
#define ERR_USERNOTINCHANNEL_FORMAT "441 %s %s :They aren't on that channel\r\n"
// ? "442 <channel> :You're not on that channel"
#define ERR_NOTONCHANNEL_FORMAT "442 %s :You're not on that channel\r\n"
// ? "443 <user> <channel> :is already on channel"
#define ERR_USERONCHANNEL_FORMAT "443 %s %s :is already on channel\r\n"
// ? "444 <user> :User not logged in"
#define ERR_NOLOGIN_FORMAT "444 %s :User not logged in\r\n"
// ? "445 :SUMMON has been disabled"
#define ERR_SUMMONDISABLED_FORMAT "445 :SUMMON has been disabled\r\n"
// ? "446 :USERS has been disabled"
#define ERR_USERSDISABLED_FORMAT "446 :USERS has been disabled\r\n"
// ? "451 :You have not registered"
#define ERR_NOTREGISTERED_FORMAT "451 :You have not registered\r\n"
// ? "461 <command> :Not enough parameters"
#define ERR_NEEDMOREPARAMS_FORMAT "461 %s :Not enough parameters\r\n"
// ? "462 :Unauthorized command (already registered)"
#define ERR_ALREADYREGISTRED_FORMAT "462 :Unauthorized command (already registered)\r\n"
// ? "463 :Your host isn't among the privileged"
#define ERR_NOPERMFORHOST_FORMAT "463 :Your host isn't among the privileged\r\n"
// ? "464 :Password incorrect"
#define ERR_PASSWDMISMATCH_FORMAT "464 :Password incorrect\r\n"
// ? "465 :You are banned from this server"
#define ERR_YOUREBANNEDCREEP_FORMAT "465 :You are banned from this server\r\n"
// ? "466 :You will be removed from the server"
#define ERR_YOUWILLBEBANNED_FORMAT "466 :You will be removed from the server\r\n"
// ? "467 <channel> :Channel key already set"
#define ERR_KEYSET_FORMAT "467 %s :Channel key already set\r\n"
// ? "471 <channel> :Cannot join channel (+l)"
#define ERR_CHANNELISFULL_FORMAT "471 %s :Cannot join channel (+l)\r\n"
// ? "472 <char> :is unknown mode char to me for <channel>"
#define ERR_UNKNOWNMODE_FORMAT "472 %s :is unknown mode char to me for %s\r\n"
// ? "473 <channel> :Cannot join channel (+i)"
#define ERR_INVITEONLYCHAN_FORMAT "473 %s :Cannot join channel (+i)\r\n"
// ? "474 <channel> :Cannot join channel (+b)"
#define ERR_BANNEDFROMCHAN_FORMAT "474 %s :Cannot join channel (+b)\r\n"
// ? "475 <channel> :Cannot join channel (+k)"
#define ERR_BADCHANNELKEY_FORMAT "475 %s :Cannot join channel (+k)\r\n"
// ? "476 <channel> :Bad Channel Mask"
#define ERR_BADCHANMASK_FORMAT "476 %s :Bad Channel Mask\r\n"
// ? "477 <channel> :Channel doesn't support modes"
#define ERR_NOCHANMODES_FORMAT "477 %s :Channel doesn't support modes\r\n"
// ? "478 <channel> <char> :Channel list is full"
#define ERR_BANLISTFULL_FORMAT "478 %s %s :Channel list is full\r\n"
// ? "481 :Permission Denied- You're not an IRC operator"
#define ERR_NOPRIVILEGES_FORMAT "481 :Permission Denied- You're not an IRC operator\r\n"
// ? "482 <channel> :You're not channel operator"
#define ERR_CHANOPRIVSNEEDED_FORMAT "482 %s :You're not channel operator\r\n"
// ? "483 :You can't kill a server!"
#define ERR_CANTKILLSERVER_FORMAT "483 :You can't kill a server!\r\n"
// ? "484 :Your connection is restricted!"
#define ERR_RESTRICTED_FORMAT "484 :Your connection is restricted!\r\n"
// ? "485 :You're not the original channel operator"
#define ERR_UNIQOPPRIVSNEEDED_FORMAT "485 :You're not the original channel operator\r\n"
// ? "491 :No O-lines for your host"
#define ERR_NOOPERHOST_FORMAT "491 :No O-lines for your host\r\n"
// ? "501 :Unknown MODE flag"
#define ERR_UMODEUNKNOWNFLAG_FORMAT "501 :Unknown MODE flag\r\n"
// ? "502 :Cannot change mode for other users"
#define ERR_USERSDONTMATCH_FORMAT "502 :Cannot change mode for other users\r\n"

#define HEADER_CUSTOM			0

#define RPL_CUSTOM				0 
#define RPL_WELCOME				1 
#define RPL_YOURHOST			2 
#define RPL_CREATED				3 
#define RPL_MYINFO				4 
#define RPL_BOUNCE				5 
#define RPL_TRACELINK			200 
#define RPL_TRACECONNECTING		201 
#define RPL_TRACEHANDSHAKE		202 
#define RPL_TRACEUNKNOWN		203 
#define RPL_TRACEOPERATOR		204 
#define RPL_TRACEUSER			205 
#define RPL_TRACESERVER			206
#define RPL_TRACESERVICE		207 
#define RPL_TRACENEWTYPE		208 
#define RPL_TRACECLASS			209
#define RPL_TRACERECONNECT		210 
#define RPL_STATSLINKINFO		211 
#define RPL_STATSCOMMANDS		212 
#define RPL_ENDOFSTATS			219 
#define RPL_UMODEIS				221 
#define RPL_SERVLIST			234 
#define RPL_SERVLISTEND			235 
#define RPL_STATSUPTIME			242 
#define RPL_STATSOLINE			243 
#define RPL_LUSERCLIENT			251 
#define RPL_LUSEROP				252 
#define RPL_LUSERUNKNOWN		253 
#define RPL_LUSERCHANNELS		254 
#define RPL_LUSERME				255 
#define RPL_ADMINME				256 
#define RPL_ADMINLOC1			257 
#define RPL_ADMINLOC2			258 
#define RPL_ADMINEMAIL			259 
#define RPL_TRACELOG			261 
#define RPL_TRACEEND			262 
#define RPL_TRYAGAIN			263 
#define RPL_AWAY				301 
#define RPL_USERHOST			302 
#define RPL_ISON				303 
#define RPL_UNAWAY				305 
#define RPL_NOWAWAY				306 
#define RPL_WHOISUSER			311 
#define RPL_WHOISSERVER			312 
#define RPL_WHOISOPERATOR		313 
#define RPL_WHOWASUSER			314 
#define RPL_ENDOFWHO			315 
#define RPL_WHOISIDLE			317 
#define RPL_ENDOFWHOIS			318 
#define RPL_WHOISCHANNELS		319 
#define RPL_LISTSTART			321 
#define RPL_LIST				322 
#define RPL_LISTEND				323 
#define RPL_CHANNELMODEIS		324 
#define RPL_UNIQOPIS			325 
#define RPL_NOTOPIC				331 
#define RPL_TOPIC				332 
#define RPL_INVITING			341 
#define RPL_SUMMONING			342 
#define RPL_INVITELIST			346 
#define RPL_ENDOFINVITELIST		347 
#define RPL_EXCEPTLIST			348 
#define RPL_ENDOFEXCEPTLIST		349 
#define RPL_VERSION				351 
#define RPL_WHOREPLY			352 
#define RPL_NAMREPLY			353 
#define RPL_LINKS				364 
#define RPL_ENDOFLINKS			365 
#define RPL_ENDOFNAMES			366 
#define RPL_BANLIST				367 
#define RPL_ENDOFBANLIST		368 
#define RPL_ENDOFWHOWAS			369 
#define RPL_INFO				371 
#define RPL_MOTD				372 
#define RPL_ENDOFINFO			374 
#define RPL_MOTDSTART			375 
#define RPL_ENDOFMOTD			376 
#define RPL_YOUREOPER			381 
#define RPL_REHASHING			382 
#define RPL_YOURESERVICE		383 
#define RPL_TIME				391 
#define RPL_USERSSTART			392 
#define RPL_USERS				393 
#define RPL_ENDOFUSERS			394 
#define RPL_NOUSERS				395 
#define ERR_NOSUCHNICK			401 
#define ERR_NOSUCHSERVER		402 
#define ERR_NOSUCHCHANNEL		403 
#define ERR_CANNOTSENDTOCHAN	404 
#define ERR_TOOMANYCHANNELS		405 
#define ERR_WASNOSUCHNICK		406 
#define ERR_TOOMANYTARGETS		407 
#define ERR_NOSUCHSERVICE		408 
#define ERR_NOORIGIN			409 
#define ERR_NORECIPIENT			411 
#define ERR_NOTEXTTOSEND		412 
#define ERR_NOTOPLEVEL			413 
#define ERR_WILDTOPLEVEL		414 
#define ERR_BADMASK				415 
#define ERR_UNKNOWNCOMMAND		421 
#define ERR_NOMOTD				422 
#define ERR_NOADMININFO			423 
#define ERR_FILEERROR			424 
#define ERR_NONICKNAMEGIVEN		431 
#define ERR_ERRONEUSNICKNAME	432 
#define ERR_NICKNAMEINUSE		433 
#define ERR_NICKCOLLISION		436 
#define ERR_UNAVAILRESOURCE		437 
#define ERR_USERNOTINCHANNEL	441 
#define ERR_NOTONCHANNEL		442 
#define ERR_USERONCHANNEL		443 
#define ERR_NOLOGIN				444 
#define ERR_SUMMONDISABLED		445 
#define ERR_USERSDISABLED		446 
#define ERR_NOTREGISTERED		451 
#define ERR_NEEDMOREPARAMS		461 
#define ERR_ALREADYREGISTRED	462 
#define ERR_NOPERMFORHOST		463 
#define ERR_PASSWDMISMATCH		464 
#define ERR_YOUREBANNEDCREEP	465 
#define ERR_YOUWILLBEBANNED		466 
#define ERR_KEYSET				467 
#define ERR_CHANNELISFULL		471 
#define ERR_UNKNOWNMODE			472 
#define ERR_INVITEONLYCHAN		473 
#define ERR_BANNEDFROMCHAN		474 
#define ERR_BADCHANNELKEY		475 
#define ERR_BADCHANMASK			476 
#define ERR_NOCHANMODES			477 
#define ERR_BANLISTFULL			478 
#define ERR_NOPRIVILEGES		481 
#define ERR_CHANOPRIVSNEEDED	482 
#define ERR_CANTKILLSERVER		483 
#define ERR_RESTRICTED			484 
#define ERR_UNIQOPPRIVSNEEDED	485 
#define ERR_NOOPERHOST			491 
#define ERR_UMODEUNKNOWNFLAG	501 
#define ERR_USERSDONTMATCH		502 

class CommandExec {
	private:

	public:
		CommandExec(void);
		~CommandExec(void);

		Send*	send_op;
		void	welcome(User* user);
		void	join(User* user, vector<string> args);
		void	part(User* user, vector<string> args);
		void	quit(User* user, vector<string> args);
		void	nick(User* user, vector<string> args);
		void	topic(User *user, vector<string> args);
		void	privmsg(User* user, vector<string> args);
		void	oper(User* user, vector<string> args);
		void	kick(User* user, vector<string> args);
		void	away(User* user, vector<string> args);
		void	names(User* user, vector<string> args);
		// void	mode(User* user, vector<string> args);
		// void	topic(t_params *params, string args, string topic);
		// void	privateMessage(t_params *params, string args, string message);
		// void	oper(string message, t_params* params);
		// void	kick(string message, t_params* params);
		// void	mode(string message, t_params* params);
		// void	user(t_params * params, string args);
};

#endif