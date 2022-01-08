#pragma once

#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <iomanip>
#include "Global.h"
#include "LRAnalysisTable.h"
#include "SymbolTable.h"
#include "Global.h"

using std::vector;
using std::stack;
using std::setw;

class Parser;

class AttGrammer { // �����ķ�
public:
	string left;
	vector<string> right;
	bool (*function)(Parser* parser); // ���嶯�� (����ָ��)

	AttGrammer() : function(nullptr) { }
	AttGrammer(const string& _left, const vector<string>& _right, bool (*f)(Parser* parser) = nullptr) : left(_left), right(_right), function(f) { }
};

class Parser {
private:
	LRAnalysisTable LRTable; // LR������
	vector<Quaternion> quaternions; // ��Ԫʽ
	stack<int> states; // ״̬ջ
	stack<string> symbols; // ����ջ

	int tempId, terminalId;

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
	void print_line(int line);
	string getNewTemp();
};

Parser::Parser() {
	nextQuad = 0;
	tempId = 0;
	terminalId = 0;
}

string Parser::getNewTemp() {
	string newTemp = "T" + std::to_string(tempId);
	tempId++;
	return newTemp;
}

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
		Action action;
		if (isNumber(symbol)) { // �ж�symbol�Ƿ�Ϊ����
			Attribute& terminal = symbolTabel.getAttribute("id");
			terminal.place = symbol;
			terminal.type = string("Number");
			action = LRTable.getAction(s, string("id"));
		}
		else if (isBool(symbol)) { // �ж�symbol�Ƿ�Ϊ������
			Attribute& terminal = symbolTabel.getAttribute("id");
			terminal.place = symbol;
			terminal.type = string("Bool");
			action = LRTable.getAction(s, string("id"));
		}
		else if (ifgoto(symbol)) {
			Attribute& terminal = symbolTabel.getAttribute("goto");
			terminal.place = symbol;
			terminal.type = string("goto");
			action = Act::Acc;
			p++;
			Quaternion q = Quaternion("j", "-", "-", str[p]);
			quaternions.push_back(q);
			
		}
		else if (ifVariable(symbol)) { // �ж�symbol�Ƿ�Ϊ�Ϸ�����
			Attribute& terminal = symbolTabel.getAttribute("id");
			terminal.place = symbol;
			terminal.type = string("Variable");
			action = LRTable.getAction(s, string("id"));
		}
		else if (ifRelop(symbol)) {
			Attribute& terminal = symbolTabel.getAttribute("relop");
			terminal.op = symbol;
			terminal.type = string("Relop");
			action = LRTable.getAction(s, string("relop"));
		}
		else action = LRTable.getAction(s, symbol); // ƥ�������ؼ���

		switch (action.act)
		{
			case Act::Shift: { // �ƽ�
				states.push(action.state);
				symbols.push(symbol);
				p++; // ָ����һ���ַ�
				break;
			}

			case Act::Reduce: { // ��Լ
				AttGrammer grammer = attGrammers[action.state]; // ��Ӧ�����ķ�
				if (grammer.function) {
					if (grammer.function(this)) { } // ִ�ж�Ӧ���嶯��
					else fail = true;
				}
				for (int i = 0; i < grammer.right.size(); i++) {
					states.pop();
					symbols.pop();
				}
				int nextState = LRTable.getGoto(states.top(), grammer.left).state;
				if (nextState < 0) { // ��Լ״̬������, ����
					fail = true;
					break;
				}
				else {
					states.push(nextState);
					symbols.push(grammer.left);
					break;
				}
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
		std::cout << setw(4) << quaternion.op << setw(10) << quaternion.arg1 << setw(10) << quaternion.arg2 << setw(5) << quaternion.result << std::endl;
	}
}
//        ��ӡgoto������һ��
void Parser::print_line(int line) {
	
	std::cout << setw(4) << quaternions[line].op << setw(10) << quaternions[line].arg1 << setw(10) << quaternions[line].arg2 << setw(5) << quaternions[line].result << std::endl;
	
}