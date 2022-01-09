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

class AttGrammer { // 属性文�?
public:
	string left;
	vector<string> right;
	bool (*function)(Parser* parser); // 语义动作 (函数指针)

	AttGrammer() : function(nullptr) { }
	AttGrammer(const string& _left, const vector<string>& _right, bool (*f)(Parser* parser) = nullptr) : left(_left), right(_right), function(f) { }
};

class Parser {
private:
	LRAnalysisTable LRTable; // LR分析�?
	vector<Quaternion> quaternions; // 四元�?

	stack<int> states; // 状态栈
	stack<string> symbols; // 符号�?

	int tempId, terminalId;

public:
	int nextQuad; // ��һ����Ԫʽ�ı��
	vector<AttGrammer> attGrammers; // �����ķ�
	SymbolTable symbolTabel; // ���ű�
	SymbolTable variableTable; // ������

	Parser();
	Parser(const string& filePath);

	void init();
	bool parse(const vector<string>& str);

	void emit(const string& op, const string& arg1, const string& arg2, const string& result);
	int makeList(const int& i);
	void pop();
	int merge(const int& p1, const int& p2);
	void backPatch(const int& p, const int& t);
	void print();
	string getNewTemp();
};

Parser::Parser(const string& filePath) : LRTable(filePath) {
	nextQuad = 0;
	tempId = 0;
	terminalId = 0;
}

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

void Parser::pop() {
	quaternions.pop_back();
	nextQuad--;
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
		quaternions[_p].result = std::to_string(t); // 回填
		_p = next_p;
	}
	quaternions[_p].result = std::to_string(t); // 回填
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
	int p = 0; // 字符串指�?
	bool fail = false, accept = false;
	while (1) {
		int s = states.top();
		string symbol = str[p];
		Action action;
		if (isNumber(symbol)) { // 判断symbol是否为数�?
			Attribute& terminal = symbolTabel.getAttribute("id");
			terminal.place = symbol;
			terminal.type = string("Number");
			action = LRTable.getAction(s, string("id"));
		}
		else if (isBool(symbol)) { // 判断symbol是否为布尔型
			Attribute& terminal = symbolTabel.getAttribute("id");
			terminal.place = symbol;
			terminal.type = string("Bool");
			action = LRTable.getAction(s, string("id"));
		}
		else if (ifRelop(symbol)) { // �ж�symbol�Ƿ�Ϊ�Ϸ���ϵ�����
			Attribute& terminal = symbolTabel.getAttribute("relop");
			terminal.op = symbol;
			terminal.type = string("Relop");
			action = LRTable.getAction(s, string("relop"));
		}
		else if (ifKeywords(symbol)) { // �ж�symbol�Ƿ�Ϊ�Ϸ��ؼ���
			action = LRTable.getAction(s, symbol);
		}
		else if (ifVariable(symbol)) { // 判断symbol是否为合法变�?
			Attribute& terminal = symbolTabel.getAttribute("id");
			terminal.place = symbol;
			terminal.type = string("Variable");
			action = LRTable.getAction(s, string("id"));
		}
		else action = LRTable.getAction(s, symbol); // ƥ�������ؼ���

		switch (action.act)
		{
			case Act::Shift: { // 移进
				states.push(action.state);
				symbols.push(symbol);
				p++; // 指向下一个字�?
				break;
			}

			case Act::Reduce: { // 规约
				AttGrammer grammer = attGrammers[action.state]; // 对应属性文�?
				if (grammer.function) {
					fail = !grammer.function(this); // ִ�ж�Ӧ���嶯��
				}
				for (int i = 0; i < grammer.right.size(); i++) {
					states.pop();
					symbols.pop();
				}
				int nextState = LRTable.getGoto(states.top(), grammer.left).state;
				if (nextState < 0) { // 规约状态不存在, 出错
					fail = true;
					break;
				}
				else {
					states.push(nextState);
					symbols.push(grammer.left);
					break;
				}
			}

			case Act::Acc: { // 接受
				accept = true;
				break;
			}

			case Act::Fail: { // 出错
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
	cout << "\n�м����:" << endl;
	std::cout << setw(8) << "op" << setw(10) << "arg1" << setw(10) << "arg2" << setw(7) << "result" << std::endl;
	int len = quaternions.size();
	for (int i = 0; i < len; i++) {
		std::cout << setw(4) << i << setw(4) << quaternions[i].op << setw(10) << quaternions[i].arg1 << setw(10) << quaternions[i].arg2 << setw(5) << quaternions[i].result << std::endl;
	}
}
