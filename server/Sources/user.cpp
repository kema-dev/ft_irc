#include "../Classes/user.hpp"
#include "../Classes/uid_pool.hpp"

using namespace std;

User::User(string name, string role) {
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
	_name = name;
	try {
		if (role == "") {
			throw (WrongUserName());
		}
		if (role != "user" && role != "operator") {
			throw (WrongUserName());
		}
	}
	catch (WrongRoleName& e) {
		cerr << e.info() << std::endl;
		return ;
	}
	_role = role;
	_nb_msg = 0;
	_ban_status = false;
	try {
		_uid = _pool.generate();
	}
	catch (PoolFull& e) {
		std::cerr << e.info() << std::endl;
		return ;
	}
}
