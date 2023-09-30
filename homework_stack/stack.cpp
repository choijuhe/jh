#include "stack.h"
#include <iostream>

using namespace std;


    Stack::Stack() {};
    void Stack::push(int number) {
        if (sizeof(like_stack) == STACK_MAX_SIZE) {
            cout << "������ �� á���ϴ�." << endl;
        }
        else {
            like_stack[++top] = number;
        }
    }
    void Stack::pop() {
        if (top == -1) {
            cout << "������ ����ֽ��ϴ�." << endl;
        }
        else {
            cout << "������ top�� : " << like_stack[top] << endl;
            top--;
        }
    }
    void Stack::is_empty() {
        if (top == -1) {
            cout << "1(�������)" << endl;
        }
        else {
            cout << "0(�����������)" << endl;
        }
    }
    void Stack::size() {
        if (top == -1) {
            cout << "������ ����ֽ��ϴ�." << endl;
        }
        else {
            cout << "���� stack�� size�� : " << top + 1 << endl;
        }
    }
    void Stack::peek() {
        if (top == -1) {
            cout << "������ ����ֽ��ϴ�." << endl;
        }
        else {
            cout << "���� stack�� top�� ��ġ�� ���� : " << like_stack[top] << endl;
        }
    }
