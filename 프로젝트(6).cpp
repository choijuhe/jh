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
		cout << "��� : " << result_number << endl;
	}
};

class Sub : public Calculator {
public:
	Sub() {
		result_number = result_number;
	}
	void result() {
		result_number -= second_number;
		cout << "��� : " << result_number << endl;
	}

};

class Div : public Calculator {
public:
	Div() {
		result_number = result_number;
	}
	void result() {
		result_number /= second_number;
		cout << "��� : " << result_number << endl;
	}

};

class Mul : public Calculator {
public:
	Mul() {
		result_number = result_number;
	}
	void result() {
		result_number *= second_number;
		cout << "��� : " << result_number << endl;
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



		cout << "���ڸ� �Է����ּ���. : ";
		cin >> c.first_number;
		c.calculator_helper();
		cout << "�����ڸ� �Է����ּ���. : ";
		cin >> user_operator;
		cout << "���ڸ� �Է����ּ���. : ";
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
			cout << "������ ��� �����Ͻðڽ��ϱ�? (Y: ���, AC: �ʱ�ȭ, EXIT: ����) ";
			cin >> progress_status;
			for (char c : progress_status) {
				progress_status_to_upper += toupper(c);
			}

			if (progress_status_to_upper == "EXIT") {
				cout << "���⸦ �����մϴ�." << endl;
				break;
			}
			else if (progress_status_to_upper == "Y") {
				cout << "�����ڸ� �Է����ּ���. : ";
				cin >> user_operator;
				cout << "���ڸ� �Է����ּ���. : ";
				cin >> c.second_number;

			}
			else{
				c.result_number = 0;
				cout << "���ڸ� �Է����ּ���. : ";
				cin >> c.first_number;
				c.calculator_helper();
				cout << "�����ڸ� �Է����ּ���. : ";
				cin >> user_operator;
				cout << "���ڸ� �Է����ּ���. : ";
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

//�ҹ��� y�� �Է��ϴ� ���..->�߾�
