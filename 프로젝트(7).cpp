#include <iostream>
#include <vector>
#include <string>
#include "character.h"
#include "weapon.h"
#include "knife.h"
#include "gun.h"

using namespace std;


using namespace std;

int main() {
	Character c;
	vector <Character*> vc;
	//vector <Character> all_character;
	//Weapon w;
	//�߻�Ŭ������ ���������Լ��� 1�� �̻� ���� �ִ� Ŭ������ ���Ѵ�.
	//�׷��� ��ó�� weapon�� ��ü�� ����� �� ����.

	vector <Weapon*> vw;

	Gun g;
	Knife k;

	int user_choose;
	string character_name;
	int character_count = 0;
	int character_count_helper = 0;

	int weapon_choose;
	int want_to_attack_character;
	vw.push_back(&k);
	vw.push_back(&g);



	cout << "ĳ������ �г����� �����ּ���. : ";
	cin >> character_name;
	vc.push_back(new Character(character_name));
	

	while (1) {
	
		for (character_count_helper = 0; character_count_helper < vc.size(); character_count_helper++) {
			if (vc[character_count_helper]->getHp() <= 0) {
				vc.erase(vc.begin() + character_count_helper);
			}

		}
	
		cout << "���ϴ� ��ȣ�� �Է����ּ���. (0:�������� | 1:���� �ݱ� | 2: ���� �ϱ� | 3: ĳ���� �߰��ϱ� | 4. ��� ĳ���� ���� Ȯ��) : ";
		cin >> user_choose;

		switch (user_choose) {
		case 0:
			cout << "������ ����Ǿ����ϴ�. " << endl;
			break;
		case 1:
			cout << "� ���⸦ �ݰڽ��ϱ�? (1. Į | 2. ��) : ";
			cin >> weapon_choose;
			vc[0]->Character::pick_weapon(weapon_choose);
			break;
		case 2:
			cout << "���� ���� : Į " << vc[0]->weapon_knife << "��, �� " << vc[0]->weapon_gun << "��" << endl;
			cout << "� ����� �����Ͻðڽ��ϱ�? (1. Į | 2. ��) : ";
			cin >> weapon_choose;
			if (c.get_weapon(weapon_choose) == 0) {
				cout << "���⸦ ������ ���� �ʽ��ϴ�." << endl;
				break;
			}
			else {
				vw[weapon_choose - 1]->attack();
				//c.use_weapon(weapon_choose);
			}
			
			cout << "�� �� ĳ���͸� �����Ͻðڽ��ϱ�? (���� ĳ���� �� :" << character_count << "��) : " << endl;
			cin >> want_to_attack_character;
			if (want_to_attack_character <= 0 || want_to_attack_character > character_count)
			{
				cout << "�ڱ� �ڽ��� ������ �� �����ϴ�." << endl;//�� �ȳ���.
				break;
			}
			else
			{
				vc[want_to_attack_character]->attack_hp(vw[weapon_choose - 1]->get_attack_power());
				c.use_weapon(weapon_choose);
			}
			break;
		case 3:
			cout << "�߰��� ĳ������ �г����� �Է����ּ���. : ";
			cin >> character_name;
			vc.push_back(new Character(character_name));
			character_count++;
			break;
		case 4:
			cout << "��� ĳ������ �г��Ӱ� hp�Դϴ�." << endl;
			for (Character* c : vc) {
				cout << "�г��� : " << c->getName() << " hp : " << c->getHp() << endl;
			}
			break;
		}
		if (user_choose == 0) {
			break;
		}
	}
	

	return 0;
}
//���̶� Į�̶� �ݴ�� ����->�߾�
//���̶� Į�� ��� ������ ������->�߾�
//�ڱ� �ڽ��� ���� ���ϰ�->�߾�
//�� ���⸦ �Է��ϰ�, ĳ���͸� �ڱ� �ڽ��� ��� ������ �ȵƾ ���Ⱑ �Һ��->�߾�
