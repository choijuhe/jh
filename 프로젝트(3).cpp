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
	
	cout << "���ڴ� 1 ~ 25 ������ ���ڸ� �Է��� �� �ֽ��ϴ�. " << endl;
	while (p_number < 7) {
		cout << p_number << "��° ��ȣ�� �Է��ϼ��� : ";
		cin >> person_num;

		if (!cin) {//!cin �ƴϸ� cin.fail()�� ���ش�.
			cout << "���ڸ� �Է����ּ���! �Ф���" << endl;
			cin.clear();//�ٽ� cin���� �Է¹��� �� �ֵ���
			cin.ignore(INT_MAX, '\n');//���Ͱ� ���ö����� ����
			continue;
		}
		//���� ���ǹ��� �Ʒ� ���ǹ����� �Ʒ��� ������ �Ʒ� ���ǹ��� continue������ ���ѹݺ����� ������.

		if (person_num > 25 || person_num <= 0) {
			cout << "1 ~ 25 ������ ���ڸ� �Է����ּ���!!" << endl;
			continue;
		}


		if (p_uniq_end_25[person_num - 1] != 1) {
			p_uniq_end_25[person_num - 1] = 1;
			all_person_num.push_back(person_num);
			p_number++;
		}
		else {
			cout << "�̹� �Էµ� �����Դϴ�." << endl;
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

	cout << "��÷��ȣ ����!" << endl;

	for (int prize : all_computer_num_paste) {
			cout << prize << " ";
	}

	cout << endl << "1 ~ 7����� ����� ���� �� �ֽ��ϴ�." << endl;
	cout << "��� : ";

	switch (same_num_count) {
	case 0:
		cout << "7���Դϴ�. �ФФФФФФ�";
		break;
	case 1:
		cout << "6���Դϴ�. �ФФФФФ�";
		break;
	case 2:
		cout << "5���Դϴ�. �ФФФФ�";
		break;
	case 3:
		cout << "4���Դϴ�. �ФФФ�";
		break;
	case 4:
		cout << "3���Դϴ�. �ФФ�";
		break;
	case 5:
		cout << "2���Դϴ�. �Ф�";
		break;
	case 6:
		cout << "1���Դϴ�. ��";
		break;
	}

	return 0;
}

//����ڷκ��� ���ڸ� �Է¹��� �� �ֵ���! -> �߾�