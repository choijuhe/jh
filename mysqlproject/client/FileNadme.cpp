#define _CRT_SECURE_NO_WARNINGS // 혹은 localtime_s를 사용
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

int main() {
    time_t timer;
    struct tm* t;
    timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
    t = localtime(&timer); // 포맷팅을 위해 구조체에 넣기
    string a;

    a = to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon + 1) + "-"
        + to_string(t->tm_mday);
    cout << a << endl;
    /*printf("현재 년: %d\n", t->tm_year + 1900);
    printf("현재 월: %d\n", t->tm_mon + 1);
    printf("현재 일: %d\n", t->tm_mday);
    printf("현재 시: %d\n", t->tm_hour);
    printf("현재 분: %d\n", t->tm_min);
    printf("현재 초: %d\n", t->tm_sec);*/

    cout << t->tm_year + 1900;

    return 0;
}