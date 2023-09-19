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

    cout << "끝말잇기 시작♡" << endl;
    cout << "단어를 입력하세요. : ";
    cin >> stored_word;
    all_word = stored_word;
    word_count = 1;
    clock_t start_time = clock();

    while (1) {


        cout << all_word << endl;


        cout << "다음 단어를 입력하세요. : ";
        cin >> input_word;

        clock_t end_time = clock();

        time = ((end_time - start_time) / CLOCKS_PER_SEC);

        if (time > 31) {
            cout << "타임 오버!";
            break;
        }



        if (all_word[all_word.size() - 1] != input_word[0]) {
            cout << "잘못된 입력입니다." << endl;
        }
        else if (all_word.find(input_word) != string::npos) {
            cout << "중복된 단어입니다." << endl;
        }
        else {
            all_word = all_word + "->" + input_word;
            word_count++;
        }


    }

    cout << "게임 종료!";
    cout << "총 입력한 단어 개수 : " << word_count;




    return 0;
}
//중복단어 안되게!-> 했어
//숫자 입력 안되게