#include "Log.hpp"

// ? Get local time in a pretty format
string	getTime(void) {
	time_t rawtime;
	struct tm*	timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime (buffer, 80, "%a %d %b %Y, %Hh %Mm %Ss", timeinfo);
	string out = buffer;
	out += ": ";
	return out;
}

// ? Log <str> in logfile
void	log(string str) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str;
	log << endl;
	log.close();
}

// ? Get the content of the logfile
string	getLog(void) {
	ostringstream sstr;
	ifstream file;
	file.open(LOGFILE_PATH);
	sstr << file.rdbuf();
	file.close();
	return sstr.str();
}

// ? Clear the content of the logfile
void	clearLog(void) {
	fstream log;
	log.open(LOGFILE_PATH, ios::out | ios::trunc);
	log << "";
	log.close();
}

// ? Log <action> done by <spec> and appends <str>
void	logError(string action, string spec, string str) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << RED;
	log << "Exception caught: ";
	log << LIGHT_MAGENTA;
	log << action;
	log << " (";
	log << spec;
	log << ")";
	log << ": ";
	log << LIGHT_YELLOW;
	log << str;
	log << DEFAULT;
	log << endl;
	log.close();
}
