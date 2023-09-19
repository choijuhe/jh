#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Calculator {
public:
	static float result_number;
	static float first_number;
	static float second_number;
	Calculator() {}
	void calculator_helper() {
		result_number = first_number;
	}
	virtual void result() {

	}
};
float Calculator::result_number = 0;
float Calculator::first_number = 0;
float Calculator::second_number = 0;

class Sum : public Calculator {

public:
	
	Sum() {
		result_number = result_number;
	}
	void result() {
		result_number += second_number;
		cout << "결과 : " << result_number << endl;
	}
};

class Sub : public Calculator {
public:
	Sub() {
		result_number = result_number;
	}
	void result() {
		result_number -= second_number;
		cout << "결과 : " << result_number << endl;
	}

};

class Div : public Calculator {
public:
	Div() {
		result_number = result_number;
	}
	void result() {
		result_number /= second_number;
		cout << "결과 : " << result_number << endl;
	}

};

class Mul : public Calculator {
public:
	Mul() {
		result_number = result_number;
	}
	void result() {
		result_number *= second_number;
		cout << "결과 : " << result_number << endl;
	}
};

int main() {
	Calculator c;
	Sum sm;
	Sub sb;
	Mul m;
	Div d;

	vector <Calculator*> vc;
	char user_operator;
	string progress_status;
	string progress_status_to_upper;

	vc.push_back(&sm);
	vc.push_back(&sb);
	vc.push_back(&d);
	vc.push_back(&m);



		cout << "숫자를 입력해주세요. : ";
		cin >> c.first_number;
		c.calculator_helper();
		cout << "연산자를 입력해주세요. : ";
		cin >> user_operator;
		cout << "숫자를 입력해주세요. : ";
		cin >> c.second_number;
		
		cout << "-------------------------------------------------------------------------" << endl;

		switch (user_operator) {
		case '+':
			vc[0]->result();
			break;
		case '-':
			vc[1]->result();
			break;
		case '/':
			vc[2]->result();
			break;
		case '*':
			vc[3]->result();
			break;
		}
	
		while (1) {
			cout << "연산을 계속 진행하시겠습니까? (Y: 계속, AC: 초기화, EXIT: 종료) ";
			cin >> progress_status;
			for (char c : progress_status) {
				progress_status_to_upper += toupper(c);
			}

			if (progress_status_to_upper == "EXIT") {
				cout << "계산기를 종료합니다." << endl;
				break;
			}
			else if (progress_status_to_upper == "Y") {
				cout << "연산자를 입력해주세요. : ";
				cin >> user_operator;
				cout << "숫자를 입력해주세요. : ";
				cin >> c.second_number;

			}
			else{
				c.result_number = 0;
				cout << "숫자를 입력해주세요. : ";
				cin >> c.first_number;
				c.calculator_helper();
				cout << "연산자를 입력해주세요. : ";
				cin >> user_operator;
				cout << "숫자를 입력해주세요. : ";
				cin >> c.second_number;
			}
			
			
		
			switch (user_operator) {
			case '+':
				vc[0]->result();
				break;
			case '-':
				vc[1]->result();
				break;
			case '/':
				vc[2]->result();
				break;
			case '*':
				vc[3]->result();
				break;
			}
		}


	return 0;
}

//소문자 y를 입력하는 경우..->했어
