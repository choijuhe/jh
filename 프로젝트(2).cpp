#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

int main() {
    int word_count = 0;
    string stored_word;
    string input_word;
    string all_word;
    int time = 0;

    cout << "�����ձ� ���ۢ�" << endl;
    cout << "�ܾ �Է��ϼ���. : ";
    cin >> stored_word;
    all_word = stored_word;
    word_count = 1;
    clock_t start_time = clock();

    while (1) {


        cout << all_word << endl;


        cout << "���� �ܾ �Է��ϼ���. : ";
        cin >> input_word;

        clock_t end_time = clock();

        time = ((end_time - start_time) / CLOCKS_PER_SEC);

        if (time > 31) {
            cout << "Ÿ�� ����!";
            break;
        }



        if (all_word[all_word.size() - 1] != input_word[0]) {
            cout << "�߸��� �Է��Դϴ�." << endl;
        }
        else if (all_word.find(input_word) != string::npos) {
            cout << "�ߺ��� �ܾ��Դϴ�." << endl;
        }
        else {
            all_word = all_word + "->" + input_word;
            word_count++;
        }


    }

    cout << "���� ����!";
    cout << "�� �Է��� �ܾ� ���� : " << word_count;




    return 0;
}
//�ߺ��ܾ� �ȵǰ�!-> �߾�
//���� �Է� �ȵǰ�