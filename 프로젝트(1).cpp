// kdt0911_0915.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
	srand(time(NULL));
	
	int person_choose;
	int computer_choose;
	int computer_choose_helper = 3;
	int num_end_31 = 0;
	
	string winner;
	

	while (num_end_31 < 31) {

		cout << "개수를 입력하세요 : ";
		cin >> person_choose;

		if (person_choose > 3 || person_choose < 1) {
			cout << "1~3 사이의 수를 입력하세요." << endl;
			continue;
		}

		cout << "사용자가 부른 숫자!" << endl;
		for (int p = 0; p < person_choose; p++) {
			cout << num_end_31 + 1 << endl; //num_end_31을 0으로 초기화해놨기에 1부터 출력되도록 +1을 해준다.
			num_end_31++;
			winner = "컴퓨터";//사용자가 숫자를 부르고 게임이 끝나면, 컴퓨터가 이긴 것.
		}
		

		
		if (num_end_31 >= 31) {//while문 조건문에 해당하지 않아도 끝까지 실행되기 때문에 중간에 종료되도록.
			break;
		}
		else {

		cout << "컴퓨터가 부른 숫자!" << endl;
		computer_choose = rand() % computer_choose_helper + 1;
		switch (num_end_31) {
		case 29://29까지 언급됐으면 컴퓨터는 1개 또는 2개의 숫자밖에 부르지 못한다.
			computer_choose = rand() % 2 + 1;
		case 30:
			computer_choose = 1;
		}
		
		
			for (int c = 0; c < computer_choose; c++) {
				cout << num_end_31 + 1 << endl;
				num_end_31++;
				winner = "사용자";
			}
		}
	}

	cout << "게임 종료!" << winner << "의 승리입니다.";

	return 0;
}

//문제점 : 내가 31 로 끝나도 컴퓨터 실행, 내가 29, 30 으로 끝나도 컴퓨터가 3을 고를 수 있음.
//-> num_end_31을 1로 초기한 후 for문을 3번 돌리면 num_end_31이 4가 돼서 내가 설정한 조건문에 적절하지 않다.

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
