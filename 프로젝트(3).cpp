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
	
	cout << "収切澗 1 ~ 25 猿走税 収切幻 脊径拝 呪 赤柔艦陥. " << endl;
	while (p_number < 7) {
		cout << p_number << "腰属 腰硲研 脊径馬室推 : ";
		cin >> person_num;

		if (!cin) {//!cin 焼艦檎 cin.fail()聖 潤層陥.
			cout << "収切幻 脊径背爽室推! ばさば" << endl;
			cin.clear();//陥獣 cin生稽 脊径閤聖 呪 赤亀系
			cin.ignore(INT_MAX, '\n');//殖斗亜 蟹臣凶猿走 巷獣
			continue;
		}
		//是税 繕闇庚戚 焼掘 繕闇庚左陥 焼掘拭 赤生檎 焼掘 繕闇庚税 continue凶庚拭 巷廃鋼差庚拭 匙遭陥.

		if (person_num > 25 || person_num <= 0) {
			cout << "1 ~ 25 紫戚税 収切幻 脊径背爽室推!!" << endl;
			continue;
		}


		if (p_uniq_end_25[person_num - 1] != 1) {
			p_uniq_end_25[person_num - 1] = 1;
			all_person_num.push_back(person_num);
			p_number++;
		}
		else {
			cout << "戚耕 脊径吉 収切脊艦陥." << endl;
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

	cout << "雁歎腰硲 因鯵!" << endl;

	for (int prize : all_computer_num_paste) {
			cout << prize << " ";
	}

	cout << endl << "1 ~ 7去猿走 衣引亜 蟹臣 呪 赤柔艦陥." << endl;
	cout << "衣引 : ";

	switch (same_num_count) {
	case 0:
		cout << "7去脊艦陥. ばばばばばばば";
		break;
	case 1:
		cout << "6去脊艦陥. ばばばばばば";
		break;
	case 2:
		cout << "5去脊艦陥. ばばばばば";
		break;
	case 3:
		cout << "4去脊艦陥. ばばばば";
		break;
	case 4:
		cout << "3去脊艦陥. ばばば";
		break;
	case 5:
		cout << "2去脊艦陥. ばば";
		break;
	case 6:
		cout << "1去脊艦陥. ⊂";
		break;
	}

	return 0;
}

//紫遂切稽採斗 収切幻 脊径閤聖 呪 赤亀系! -> 梅嬢