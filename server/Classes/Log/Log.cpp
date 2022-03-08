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

void	log(string str1, string str2) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
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
