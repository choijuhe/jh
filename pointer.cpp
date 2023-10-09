#include <iostream>
#include <math.h>



int main() {
	int arr[4] = { 10, 20, 30, 40 };
	int* pointer = arr;


	std::cout << *pointer << std::endl; // arr[0]
	std::cout << pointer << std::endl; // arr[0]의 주소
	std::cout << pointer + 1 << std::endl; // arr[0]의 주소 + 4(int형이니까)
	std::cout << *pointer + 1 << std::endl; // arr[0] + 1
	std::cout << *(pointer + 1) << std::endl; // arr[0]의 주소 + 4 번지가 가리키는 값 -> arr[1]

	

	return 0;
}
