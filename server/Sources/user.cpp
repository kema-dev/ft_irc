#include "../Classes/user.hpp"
#include "../Classes/uid_pool.hpp"

using namespace std;

User::User(string name, string role, string pass, UidPool pool) {
	try {
		if (name == "") {
			throw (WrongUserName());
		}
		for (size_t i = 0; name[i]; i++) {
			if (!(isalnum(name[i]))) {
				throw (WrongUserName());
			}
		}
	}
	catch (WrongUserName& e) {
		cerr << e.info() << std::endl;
		return ;
	}
	try {
		if (role == "") {
			throw (WrongUserName());
		}
		if (role != "user" && role != "operator") {
			throw (WrongUserName());
		}
	}
	catch (WrongRoleNameUser& e) {
		cerr << e.info() << std::endl;
		return ;
	}
	size_t id;
	try {
		id = pool.generate();
	}
	catch (PoolFull& e) {
		std::cerr << e.info() << std::endl;
		return ;
	}
	_name = name;
	_role = role;
	_hash = md5(pass);
	_uid = id;
	_nb_msg = 0;
	_ban_status = false;
	_active_status = true;
}

string	User::md5(string str) {
	if (str.length() < 1)
		return "";
	FILE *fpipe;
	char *command = new char[string(str).length() + 14]();
	sprintf(command, "echo '%s' | md5", str.c_str());
	char c = 0;

	if (0 == (fpipe = (FILE*)popen(command, "r")))
	{
		perror("popen() failed.");
		exit(EXIT_FAILURE);
	}
	string out;
	while (fread(&c, sizeof(c), 1, fpipe)) {
		out += c;
	}
	pclose(fpipe);
	delete command;
	return out;
}

string	User::getName(void) {
	return _name;
}

string	User::getRole(void) {
	return _role;
}

ssize_t	User::getNbMsg(void) {
	return _nb_msg;
}

bool	User::getBanStatus(void) {
	return _ban_status;	
}

bool	User::getActiveStatus(void) {
	return _active_status;	
}

ssize_t	User::getUid(void) {
	return _uid;	
}

string	User::getHash(void) {
	return _hash;	
}

std::ostream &operator<<(std::ostream &stream, User &rhs)
{
	stream << "User infos:" << endl << "name: " << rhs.getName() << endl << "role: " << rhs.getRole() << endl << "nb_msg: " << rhs.getNbMsg() << endl << "ban_status: " << rhs.getBanStatus() << endl << "active_status: " << rhs.getActiveStatus() << endl << "uid: " << rhs.getUid() << endl << "hash: " << rhs.getHash();
    return stream;
}

bool	User::setName(string new_name) {
	_name = new_name;
	return true;
}

bool	User::setRole(string new_role) {
	_role = new_role;
	return true;
}

bool	User::setNbMsg(ssize_t new_nb_msg) {
	_nb_msg = new_nb_msg;
	return true;
}

bool	User::setBanStatus(bool new_ban_status) {
	_ban_status = new_ban_status;
	return true;
}

bool	User::setActiveStatus(bool new_active_status) {
	_active_status = new_active_status;
	return true;
}

bool	User::setUid(ssize_t new_uid) {
	_uid = new_uid;
	return true;
}

bool	User::setHash(string new_hash) {
	_hash = new_hash;
	return true;
}

bool	User::setPass(string new_pass) {
	_hash = md5(new_pass);
	return true;
}

bool	User::log_in(string pass) {
	if (md5(pass) == getHash())
		return true;
	return false;
}

int main(void) {
	string name("name");
	string role("user");
	string pass("password");
	UidPool	pool = UidPool();
	User usr = User(name, role, pass, pool);
	cout << usr << endl << endl;
	if (usr.log_in("password") == true)
		cout << "true" << endl;
	else
		cout << "false" << endl;
}


// TODO ANCHOR User database / User search in db
// TODO ANCHOR Dont forget to branch