#ifndef _LOG_HPP_
 #define _LOG_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#define LOGFILE_PATH "@PATHTOLOGFILE"

void	log(string str);
void	log(string str1, string str2);
void	log(string str1, string str2, string str3);
void	log(string str1, string str2, string str3, string str4);
void	log(string str1, string str2, string str3, string str4, string str5);
string	getLog(void);

#endif