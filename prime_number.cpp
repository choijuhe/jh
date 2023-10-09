#include <iostream>
#include <math.h>

int is_prime(int num) {
	int sqrt_num = 0;
	sqrt_num = (int)sqrt(num);

	for (int i = 2; i <= sqrt_num; i++) {
		if (num % i == 0) {
			return false;
		}
	}return true;
}


int main() {
	int num = 20;

	switch (is_prime(num)) {
	case true:
		std::cout << num << "��(��) " << "�Ҽ��Դϴ�." << std::endl;
		break;
	case false:
		std::cout << num << "��(��) " << "�Ҽ��� �ƴմϴ�." << std::endl;
		break;
	}
	
	return 0;
}
