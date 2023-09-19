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
	//추상클래스는 순수가상함수를 1개 이상 갖고 있는 클래스를 말한다.
	//그래서 위처럼 weapon의 개체를 사용할 수 없다.

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



	cout << "캐릭터의 닉네임을 정해주세요. : ";
	cin >> character_name;
	vc.push_back(new Character(character_name));
	

	while (1) {
	
		for (character_count_helper = 0; character_count_helper < vc.size(); character_count_helper++) {
			if (vc[character_count_helper]->getHp() <= 0) {
				vc.erase(vc.begin() + character_count_helper);
			}

		}
	
		cout << "원하는 번호를 입력해주세요. (0:게임종료 | 1:무기 줍기 | 2: 공격 하기 | 3: 캐릭터 추가하기 | 4. 모든 캐릭터 정보 확인) : ";
		cin >> user_choose;

		switch (user_choose) {
		case 0:
			cout << "게임이 종료되었습니다. " << endl;
			break;
		case 1:
			cout << "어떤 무기를 줍겠습니까? (1. 칼 | 2. 총) : ";
			cin >> weapon_choose;
			vc[0]->Character::pick_weapon(weapon_choose);
			break;
		case 2:
			cout << "현재 무기 : 칼 " << vc[0]->weapon_knife << "개, 총 " << vc[0]->weapon_gun << "개" << endl;
			cout << "어떤 무기로 공격하시겠습니까? (1. 칼 | 2. 총) : ";
			cin >> weapon_choose;
			if (c.get_weapon(weapon_choose) == 0) {
				cout << "무기를 가지고 있지 않습니다." << endl;
				break;
			}
			else {
				vw[weapon_choose - 1]->attack();
				//c.use_weapon(weapon_choose);
			}
			
			cout << "몇 번 캐릭터를 공격하시겠습니까? (현재 캐릭터 수 :" << character_count << "명) : " << endl;
			cin >> want_to_attack_character;
			if (want_to_attack_character <= 0 || want_to_attack_character > character_count)
			{
				cout << "자기 자신은 공격할 수 없습니다." << endl;//왜 안나와.
				break;
			}
			else
			{
				vc[want_to_attack_character]->attack_hp(vw[weapon_choose - 1]->get_attack_power());
				c.use_weapon(weapon_choose);
			}
			break;
		case 3:
			cout << "추가할 캐릭터의 닉네임을 입력해주세요. : ";
			cin >> character_name;
			vc.push_back(new Character(character_name));
			character_count++;
			break;
		case 4:
			cout << "모든 캐릭터의 닉네임과 hp입니다." << endl;
			for (Character* c : vc) {
				cout << "닉네임 : " << c->getName() << " hp : " << c->getHp() << endl;
			}
			break;
		}
		if (user_choose == 0) {
			break;
		}
	}
	

	return 0;
}
//총이랑 칼이랑 반대로 나옴->했어
//총이랑 칼이 없어도 공격이 가능함->했어
//자기 자신은 공격 못하게->했어
//쓸 무기를 입력하고, 캐릭터를 자기 자신을 골라서 공격이 안됐어도 무기가 소비됨->했어
