#define _CRT_SECURE_NO_WARNINGS // Ȥ�� localtime_s�� ���
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

int main() {
    time_t timer;
    struct tm* t;
    timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
    t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
    string a;

    a = to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon + 1) + "-"
        + to_string(t->tm_mday);
    cout << a << endl;
    /*printf("���� ��: %d\n", t->tm_year + 1900);
    printf("���� ��: %d\n", t->tm_mon + 1);
    printf("���� ��: %d\n", t->tm_mday);
    printf("���� ��: %d\n", t->tm_hour);
    printf("���� ��: %d\n", t->tm_min);
    printf("���� ��: %d\n", t->tm_sec);*/

    cout << t->tm_year + 1900;

    return 0;
}