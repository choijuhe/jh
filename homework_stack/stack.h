#pragma once
#include <iostream>

const int STACK_MAX_SIZE = 5;
using namespace std;

class Stack {
    int top = -1;
    int like_stack[STACK_MAX_SIZE] = {};

public:
    Stack();
    void push(int number);
    void pop();
    void is_empty();
    void size();
    void peek();

};

