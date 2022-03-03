#include "Crypto.hpp"

string	md5(string str) {
	if (str.length() < 1)
		return "";
	FILE *fpipe;
	char *command = new char[string(str).length() + 14]();
	sprintf(command, "echo '%s' | md5", str.c_str());
	char c = 0;

	if (0 == (fpipe = (FILE*)popen(command, "r"))) {
		throw (PopopenFail());
		return "";
	}
	string out;
	while (fread(&c, sizeof(c), 1, fpipe)) {
		out += c;
	}
	pclose(fpipe);
	delete command;
	return out;
}
