#ifndef _LOG_HPP_
 #define _LOG_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <time.h>

using namespace std;

//	*	formats

# define DEFAULT			"\e[0m"
# define BOLD				"\e[1m"
# define DIM				"\e[2m"
# define ITALIC				"\e[3m"
# define UNDERLINED			"\e[4m"
# define BLINK				"\e[5m"
# define REVERSE			"\e[7m"
# define HIDDEN				"\e[8m"

# define NO_FORMAT			"\e[22m"

//	*	text colors

# define NO_COLOR			"\e[39m"
# define BLACK				"\e[30m"
# define RED				"\e[31m"
# define GREEN				"\e[32m"
# define YELLOW				"\e[33m"
# define BLUE				"\e[34m"
# define MAGENTA			"\e[35m"
# define CYAN				"\e[36m"
# define LIGHT_GRAY			"\e[37m"
# define DARK_GRAY			"\e[90m"
# define LIGHT_RED			"\e[91m"
# define LIGHT_GREEN		"\e[92m"
# define LIGHT_YELLOW		"\e[93m"
# define LIGHT_BLUE			"\e[94m"
# define LIGHT_MAGENTA		"\e[95m"
# define LIGHT_CYAN			"\e[96m"
# define WHITE				"\e[97m"

//	*	background colors

# define BACK_NO_COLOR		"\e[49m"
# define BACK_BLACK			"\e[40m"
# define BACK_RED			"\e[41m"
# define BACK_GREEN			"\e[42m"
# define BACK_YELLOW		"\e[43m"
# define BACK_BLUE			"\e[44m"
# define BACK_MAGENTA		"\e[45m"
# define BACK_CYAN			"\e[46m"
# define BACK_LIGHT_GRAY	"\e[47m"
# define BACK_DARK_GRAY		"\e[100m"
# define BACK_LIGHT_RED		"\e[101m"
# define BACK_LIGHT_GREEN	"\e[102m"
# define BACK_LIGHT_YELLOW	"\e[103m"
# define BACK_LIGHT_BLUE	"\e[104m"
# define BACK_LIGHT_MAGENTA	"\e[105m"
# define BACK_LIGHT_CYAN	"\e[106m"
# define BACK_WHITE			"\e[107m"

#define LOGFILE_PATH "@PATHTOLOGFILE"

void	log(string str);
void	logError(string action, string spec, string str);
string	getTime(void);
string	getLog(void);
void	clearLog(void);

#endif