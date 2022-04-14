#include "Crypto.hpp"

// ? Hash <str> using SHA256
string	sha256(string str) {
	if (str.empty())
		return "";
	for (size_t i = 0; str[i]; i++) {
		if (!(isalnum(str[i]))) {
			throw (NotAlnum());
		}
	}
	FILE *fpipe;
	char *command = new char[string(str).length() + 36]();
	sprintf(command, "echo %s | shasum -a 256 | tr -d ' -\n'", str.c_str());
	char c = 0;

	if (0 == (fpipe = (FILE*)popen(command, "r"))) {
		delete command;
		throw (PopopenFail());
	}
	string out;
	while (fread(&c, sizeof(c), 1, fpipe)) {
		out += c;
	}
	pclose(fpipe);
	delete command;
	return out;
}
