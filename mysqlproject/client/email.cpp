#include <iostream>
#include <ctime>
#include <string>
using namespace std;
const string id;
void check1(string*);

void check(string* a) {
	cout << *a << endl;
	check1(a);
	cout << *a << endl;
}
void check1(string* a) {
	*a = "�ȳ�";
}

int main() {
	string a = "�ʹ���";
	string b;
	
	
	check(&a);

	
}