#include <iostream>
#include <vector>
using namespace std;

int main() {
	int odd_num;
	int ma_num = 1;
	int mid;
	int row = 0;
	int col = 0;

	cout << "�������� �� Ȥ�� ���� ���� �ڿ���(Ȧ��)�� �Է����ּ���. : ";
	cin >> odd_num;

	while (1) {
		if (odd_num % 2 == 0) {
			cout << "Ȧ���� �Է����ּ���.";
			cin >> odd_num;
			continue;
		}
		else {
			break;
		}
	}

	vector <vector<int>> ma(odd_num, vector<int>(odd_num, 0));
	mid = (odd_num - 1) / 2;

	ma[0][mid] = ma_num;
	ma_num++;
	col = mid;
	//����.
	
	while (ma_num <= odd_num * odd_num) {
		
		row = row - 1; // �����ڸ��� row = -1
		col = col + 1; //�����ڸ��� col = mid + 1;
		
		
		if (row == -1) {
			row = odd_num - 1;
		}
		if (col == odd_num) {
			col = 0;
		}

		if (ma[row][col] == 0) {//ĭ�� ä���������� �Ʒ���, ������ ���̸� ù��° ������.
			ma[row][col] = ma_num;
			ma_num++;
			if (row == 0 && col == odd_num - 1) {
				row = row + 2;
				col = odd_num - 2;
			}
		}
		else {
			row = row + 2;
			col--;
			if (row == odd_num - 1 && col == odd_num - 1) {
				row++;
			}
			if (row == -1) {
				row = odd_num - 1;
			}
			if (col == odd_num) {
				col = 0;
			}
			ma[row][col] = ma_num;
			ma_num++;
		}
	}


	for (int i = 0; i < odd_num; i++) {
		for (int j = 0; j < odd_num; j++) {
			cout << ma[i][j] << " ";
		}
		cout << endl;
	}



	return 0;
}
