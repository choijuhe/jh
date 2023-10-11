#include <iostream>
#include <ctime>
#include <string>
using namespace std;

int main() {
	string a;
	
	cout << "이메일을 입력해주세요. >> ";
	while (1) {
		
		cin >> a;
		if (a.find("@") == string::npos) {
			cout << "이메일 형식에 맞춰서 입력해주세요. >> ";
			continue;
		}
		else if (a.find(".com") == string::npos) {
			cout << "이메일 형식에 맞춰서 입력해주세요. >> ";
			continue;
		}
		else if (a.find(".com") < a.find("@")) {
			cout << "이메일 형식에 맞춰서 입력해주세요. >> ";
			continue;
		}
		else
			break;
	}
	
}