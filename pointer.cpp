#include <iostream>
#include <math.h>



int main() {
	int arr[4] = { 10, 20, 30, 40 };
	int* pointer = arr;


	std::cout << *pointer << std::endl; // arr[0]
	std::cout << pointer << std::endl; // arr[0]�� �ּ�
	std::cout << pointer + 1 << std::endl; // arr[0]�� �ּ� + 4(int���̴ϱ�)
	std::cout << *pointer + 1 << std::endl; // arr[0] + 1
	std::cout << *(pointer + 1) << std::endl; // arr[0]�� �ּ� + 4 ������ ����Ű�� �� -> arr[1]

	

	return 0;
}
