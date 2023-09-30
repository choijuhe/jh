#include "stack.h"
#include <iostream>

using namespace std;


    Stack::Stack() {};
    void Stack::push(int number) {
        if (sizeof(like_stack) == STACK_MAX_SIZE) {
            cout << "스택이 꽉 찼습니다." << endl;
        }
        else {
            like_stack[++top] = number;
        }
    }
    void Stack::pop() {
        if (top == -1) {
            cout << "스택이 비어있습니다." << endl;
        }
        else {
            cout << "스택의 top은 : " << like_stack[top] << endl;
            top--;
        }
    }
    void Stack::is_empty() {
        if (top == -1) {
            cout << "1(비어있음)" << endl;
        }
        else {
            cout << "0(비어있지않음)" << endl;
        }
    }
    void Stack::size() {
        if (top == -1) {
            cout << "스택이 비어있습니다." << endl;
        }
        else {
            cout << "현재 stack의 size는 : " << top + 1 << endl;
        }
    }
    void Stack::peek() {
        if (top == -1) {
            cout << "스택이 비어있습니다." << endl;
        }
        else {
            cout << "현재 stack의 top에 위치한 값은 : " << like_stack[top] << endl;
        }
    }
