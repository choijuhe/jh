#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	srand(time(NULL));
	int p_number = 1;
	int c_number = 1;
	vector <int> all_person_num;
	vector <int> p_uniq_end_25(25, 0);
	int person_num = 0;

	int computer_num;
	vector <int> all_computer_num;
	vector <int> all_computer_num_paste;
	vector <int> c_uniq_end_25(25, 0);

	int same_num_count = 0;
	
	cout << "숫자는 1 ~ 25 까지의 숫자만 입력할 수 있습니다. " << endl;
	while (p_number < 7) {
		cout << p_number << "번째 번호를 입력하세요 : ";
		cin >> person_num;

		if (!cin) {//!cin 아니면 cin.fail()을 써준다.
			cout << "숫자만 입력해주세요! ㅠㅅㅠ" << endl;
			cin.clear();//다시 cin으로 입력받을 수 있도록
			cin.ignore(INT_MAX, '\n');//엔터가 나올때까지 무시
			continue;
		}
		//위의 조건문이 아래 조건문보다 아래에 있으면 아래 조건문의 continue때문에 무한반복문에 빠진다.

		if (person_num > 25 || person_num <= 0) {
			cout << "1 ~ 25 사이의 숫자만 입력해주세요!!" << endl;
			continue;
		}


		if (p_uniq_end_25[person_num - 1] != 1) {
			p_uniq_end_25[person_num - 1] = 1;
			all_person_num.push_back(person_num);
			p_number++;
		}
		else {
			cout << "이미 입력된 숫자입니다." << endl;
		}
	}

	while (c_number < 7) {
		computer_num = rand() % 25 + 1;

		if (c_uniq_end_25[computer_num - 1] != 1) {
			c_uniq_end_25[computer_num - 1] = 1;
			all_computer_num.push_back(computer_num);
			all_computer_num_paste.push_back(computer_num);
			c_number++;
		}
		else {
			continue;
		}
	}
	//sort(all_computer_num.begin(), all_computer_num.end());



	for (int i = 0; i < all_computer_num.size(); i++) {
		for (int j = 0; j < all_person_num.size(); j++) {
			if (all_computer_num[i] == all_person_num[j]) {
				same_num_count++;
				all_computer_num.erase(all_computer_num.begin() + i);
				i = 0;
			}
		}
	}
	sort(all_computer_num_paste.begin(), all_computer_num_paste.end());

	cout << "당첨번호 공개!" << endl;

	for (int prize : all_computer_num_paste) {
			cout << prize << " ";
	}

	cout << endl << "1 ~ 7등까지 결과가 나올 수 있습니다." << endl;
	cout << "결과 : ";

	switch (same_num_count) {
	case 0:
		cout << "7등입니다. ㅠㅠㅠㅠㅠㅠㅠ";
		break;
	case 1:
		cout << "6등입니다. ㅠㅠㅠㅠㅠㅠ";
		break;
	case 2:
		cout << "5등입니다. ㅠㅠㅠㅠㅠ";
		break;
	case 3:
		cout << "4등입니다. ㅠㅠㅠㅠ";
		break;
	case 4:
		cout << "3등입니다. ㅠㅠㅠ";
		break;
	case 5:
		cout << "2등입니다. ㅠㅠ";
		break;
	case 6:
		cout << "1등입니다. ♥";
		break;
	}

	return 0;
}

//사용자로부터 숫자만 입력받을 수 있도록! -> 했어