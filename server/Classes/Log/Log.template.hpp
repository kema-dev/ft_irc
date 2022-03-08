#ifndef _LOG_HPP_
 #define _LOG_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>

using namespace std;

#define LOGFILE_PATH "/Users/jjourdan/dev/github/ft_irc/server/log/server.log"

void	log(string str);
void	log(string str1, string str2);
void	log(string str1, string str2, string str3);
void	log(string str1, string str2, string str3, string str4);
void	log(string str1, string str2, string str3, string str4, string str5);
string	getTime(void);
string	getLog(void);
void	clearLog(void);

#endif