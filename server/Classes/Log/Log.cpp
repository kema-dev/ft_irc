#include "Log.hpp"

void	log(string str) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << str;
	log << endl;
}

string	getLog(void) {
	ostringstream sstr;
	ifstream file;
	file.open(LOGFILE_PATH);
    sstr << file.rdbuf();
    return sstr.str();
}
