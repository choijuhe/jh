#include <iostream>
#include <ctime>
#include <string>
using namespace std;

int main() {
	string a;
	
	cout << "�̸����� �Է����ּ���. >> ";
	while (1) {
		
		cin >> a;
		if (a.find("@") == string::npos) {
			cout << "�̸��� ���Ŀ� ���缭 �Է����ּ���. >> ";
			continue;
		}
		else if (a.find(".com") == string::npos) {
			cout << "�̸��� ���Ŀ� ���缭 �Է����ּ���. >> ";
			continue;
		}
		else if (a.find(".com") < a.find("@")) {
			cout << "�̸��� ���Ŀ� ���缭 �Է����ּ���. >> ";
			continue;
		}
		else
			break;
	}
	
}