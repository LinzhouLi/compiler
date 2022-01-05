#pragma once

#include <vector>
#include <stack>
#include <iostream>
#include "Global.h"
#include "LRAnalysisTable.h"
#include "SymbolTable.h"

using std::vector;
using std::stack;

class Parser;

class AttGrammer { // �����ķ�
public:
	string left;
	vector<string> right;
	void (*function)(Parser* parser); // ���嶯�� (����ָ��)

	AttGrammer() : function(nullptr) { }
	AttGrammer(const string& _left, const vector<string>& _right, void (*f)(Parser* parser) = nullptr) : left(_left), right(_right), function(f) { }
};

class Parser {
private:
	LRAnalysisTable LRTable; // LR������
	vector<Quaternion> quaternions; // ��Ԫʽ

	stack<int> states; // ״̬ջ
	stack<string> symbols; // ����ջ

public:
	int nextQuad; // ��һ����Ԫʽ�ı��
	vector<AttGrammer> attGrammers; // �����ķ�
	SymbolTable symbolTabel; // ���ű�

	Parser();

	void init();
	bool parse(const vector<string>& str);

	void emit(const string& op, const string& arg1, const string& arg2, const string& result);
	int makeList(const int& i);
	int merge(const int& p1, const int& p2);
	void backPatch(const int& p, const int& t);
	void print();
};

void Parser::emit(const string& op, const string& arg1, const string& arg2, const string& result) {
	Quaternion q = Quaternion(op, arg1, arg2, result);
	quaternions.push_back(q);
	nextQuad++;
}

int Parser::makeList(const int& i) {
	return i;
}

int Parser::merge(const int& p1, const int& p2) {
	int _p = p1;
	while (quaternions[_p].result != "0") {
		_p = atoi(quaternions[_p].result.c_str()); // string->int
	}
	quaternions[_p].result = std::to_string(p2); // int->string
	return p1;
}

void Parser::backPatch(const int& p, const int& t) {
	int _p = p;
	while (quaternions[_p].result != "0") {
		int next_p = atoi(quaternions[_p].result.c_str()); // string->int
		quaternions[_p].result = std::to_string(t); // ����
		_p = next_p;
	}
	quaternions[_p].result = std::to_string(t); // ����
}

void Parser::init() {
	quaternions.clear();
	nextQuad = 0;
	if (!states.empty()) {
		states.pop();
	}
	if (!symbols.empty()) {
		symbols.pop();
	}
}

bool Parser::parse(const vector<string>& str) {
	states.push(0);
	symbols.push("#");
	int p = 0; // �ַ���ָ��
	bool fail = false, accept = false;
	while (1) {
		int s = states.top();
		string symbol = str[p];
		Action action = LRTable.getAction(s, symbol);

		switch (action.act)
		{
			case Act::Shift: { // �ƽ�
				states.push(action.state);
				symbols.push(symbol);
				p++; // ָ����һ���ַ�
				break;
			}

			case Act::Reduce: { // ��Լ
				AttGrammer grammer = attGrammers[action.state - 1]; // ��Ӧ�����ķ�
				if (grammer.function) {
					grammer.function(this); // ִ�ж�Ӧ���嶯��
				}
				for (int i = 0; i < grammer.right.size(); i++) {
					states.pop();
					symbols.pop();
				}
				states.push( LRTable.getGoto(states.top(), grammer.left).state );
				symbols.push(grammer.left);
				break;
			}

			case Act::Acc: { // ����
				accept = true;
				break;
			}

			case Act::Fail: { // ����
				fail = true;
				break;
			}

			default: {
				fail = true;
				break;
			}
		}

		if (fail) return false;
		if (accept) return true;
	}
}

void Parser::print() {
	for (auto quaternion : quaternions) {
		std::cout << quaternion.op << "  " << quaternion.arg1 << "  " << quaternion.arg2 << "  " << quaternion.result << std::endl;
	}
}