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

void	log(string str1, string str2, string str3, string str4, string str5, string str6) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12, string str13) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << str13;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12, string str13, string str14) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << str13;
	log << str14;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12, string str13, string str14, string str15) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << str13;
	log << str14;
	log << str15;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12, string str13, string str14, string str15, string str16) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << str13;
	log << str14;
	log << str15;
	log << str16;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12, string str13, string str14, string str15, string str16, string str17) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << str13;
	log << str14;
	log << str15;
	log << str16;
	log << str17;
	log << endl;
	log.close();
}

void	log(string str1, string str2, string str3, string str4, string str5, string str6, string str7, string str8, string str9, string str10, string str11, string str12, string str13, string str14, string str15, string str16, string str17, string str18) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << str1;
	log << str2;
	log << str3;
	log << str4;
	log << str5;
	log << str6;
	log << str7;
	log << str8;
	log << str9;
	log << str10;
	log << str11;
	log << str12;
	log << str13;
	log << str14;
	log << str15;
	log << str16;
	log << str17;
	log << str18;
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

void	logError(string str1, const char* str2) {
	fstream log;
	log.open(LOGFILE_PATH, ios_base::app);
	log << getTime();
	log << "Exception caught during ";
	log << str1;
	log << ": ";
	log << str2;
	log << endl;
	log.close();
}
