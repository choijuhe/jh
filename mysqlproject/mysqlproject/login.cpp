#include <iostream>
#include <string>
#include <mysql/jdbc.h>

#pragma comment(lib, "libmySQL.lib")

using std::cout;
using std::endl;
using std::string;

class login {

public:
	void btnLogIn_Click() {
		LoadUserInfo();
		CheckID_PW();
	}
	static void LoadUserInfo() {

	}

private:
	void CheckID_PW() {

	}


};