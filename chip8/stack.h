#pragma once

#include <stdexcept>

class Stack {
public:
	void push(unsigned short s){
		if (SP > 15) throw std::invalid_argument("Stack is full.");
		stack[++SP] = s;
	}

	unsigned short pop() {
		if (SP > -1) return stack[SP--];
		throw std::invalid_argument("Stack is empty.");
	}

	Stack() {
		SP = -1;
		stack = new unsigned short[16];
		memset(stack, 0, 16 * sizeof(unsigned short));
	}
	~Stack() {
		delete stack;
	}

private:
	char SP;
	unsigned short* stack;
};