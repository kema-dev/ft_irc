#include "Log.hpp"

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

void	log(string str) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str;
	log << endl;
	log.close();
}

string	getLog(void) {
	ostringstream sstr;
	ifstream file;
	file.open(LOGFILE_PATH);
	sstr << file.rdbuf();
	file.close();
	return sstr.str();
}

void	clearLog(void) {
	fstream log;
	log.open(LOGFILE_PATH, ios::out | ios::trunc);
	log << "";
	log.close();
}

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
