#include "../UserDB/UserDB.hpp"

using namespace std;

void	UserDB::add(User usr) {
	_db.push_back(usr);
}

// int main(void) {
// 	cout << "OK" << endl;
// }
