#ifndef _LOG_HPP_
 #define _LOG_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#define LOGFILE_PATH "/Users/jjourdan/dev/github/ft_irc/server/log/server.log"

void	log(string str);
string	getLog(void);

#endif
