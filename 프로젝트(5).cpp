#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <exception>
using namespace std;

int main() {
	srand(time(NULL));
	string user_choose = "";
	string not_word;
	int computer_choose;
	int end_three = 0;
	vector <int> three_computer_choose;
	vector <int> three_user_choose;
	int string_to_int;
	int start_1_end_9 = 0;

	vector <int> number(10, 0);

	int start_1_end_9_helper = 0;
	int count = 0;
	int ball = 0;
	int strike = 0;

	while (end_three < 3) {
		computer_choose = rand() % 9 + 1;
		if (number[computer_choose] == 0) {
			three_computer_choose.push_back(computer_choose);
			end_three++;
			number[computer_choose] = 1;
		}
		else { continue; }
	}

	cout << "�߱� ����" << endl;
	
	while (1) {
		strike = 0;
		ball = 0;
		start_1_end_9_helper = 0;
		three_user_choose.clear();
		cout << "1 ~ 9 ������ ���� 3���� �Է��Ͻÿ�. (�̿��� ���� : ����)" << endl;
		getline(cin, user_choose);
		

		try {
			for (char c : user_choose) {
				not_word = c;
				stoi(not_word);
			}
		}
		catch (const exception& expn) {
			cout << "���ڸ� �Է����ּ���!" << endl;
			continue;
		}
		catch (...) {
			cout << "�����̰�" << endl;
		}
		
			count++;
		
		
		

		
		for (string_to_int = 0; string_to_int < user_choose.size();) {
			three_user_choose.push_back(user_choose[string_to_int] - '0');
			string_to_int += 2;
		}

		for (start_1_end_9 = 0; start_1_end_9 < three_user_choose.size(); start_1_end_9++) {
			if (three_user_choose[start_1_end_9] < 0 || 9 < three_user_choose[start_1_end_9]) {
				start_1_end_9_helper++;
			}
		}
		
			if (start_1_end_9_helper > 0) {
				cout << "������ ����Ǿ����ϴ�.";
				break;
			}
	
		for (int i = 0; i < 3; i++) {
			if (three_computer_choose[i] == three_user_choose[i]) {
				strike++;
				//three_computer_choose.erase(three_computer_choose.begin() + i);
				//three_user_choose.erase(three_user_choose.begin() + i);
			}
		}

		for (int i = 0; i < three_computer_choose.size(); i++) {
			for (int j = 0; j < three_user_choose.size(); j++) {
				if (three_computer_choose[i] == three_user_choose[j] && i != j) {
					ball++;
					
					three_user_choose[j] = 0;
					i = 0;
					j = 0;
				}
			}
		}
		cout << "Strike : " << strike << "     " << "Ball : " << ball << endl;

		if (strike == 3) {
			break;
		}

	}
	if (strike == 3) {
		cout << count << "�� ���� ������ϴ�.";
	}
	
	return 0;
}
//���� �Է� ���ϰ�->stoi�Լ�:�Ǿտ� ���ڰ� �ԷµǸ� �װ͸� ���������� ���������� ����
// ->�� ���� ���ڴ� �׳� ����. �׷��� ���ڰ� �ν��� �ȵ�.
//�ߺ��� ���� �Է¸��ϰ�