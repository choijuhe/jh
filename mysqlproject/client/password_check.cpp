#include <iostream>
#include <string>
using namespace std;
const int special_word = 3;

bool password_check(string a) {
	char special_char[special_word] = { '!', '?', '#' };
	for (char c : special_char) {
		if (a.find(c) != string::npos) {
			return true;
		}
	}
	return false;
}

int main() {
	string a;
	


	while (1) {
		cout << "��й�ȣ�� �Է����ּ���.(!, ?, # �� 1�� �̻� ����) >> ";
		cin >> a;
		if (password_check(a)) {
			break;
		}
		continue;
	}

	
}