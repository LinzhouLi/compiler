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

class AttGrammer { // 属性文法
public:
	string left;
	vector<string> right;
	void (*function)(Parser* parser); // 语义动作 (函数指针)

	AttGrammer() : function(nullptr) { }
	AttGrammer(const string& _left, const vector<string>& _right, void (*f)(Parser* parser) = nullptr) : left(_left), right(_right), function(f) { }
};

class Parser {
private:
	LRAnalysisTable LRTable; // LR分析表
	vector<Quaternion> quaternions; // 四元式

	stack<int> states; // 状态栈
	stack<string> symbols; // 符号栈

public:
	int nextQuad; // 下一条四元式的标号
	vector<AttGrammer> attGrammers; // 属性文法
	SymbolTable symbolTabel; // 符号表

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
	int p = 0; // 字符串指针
	bool fail = false, accept = false;
	while (1) {
		int s = states.top();
		string symbol = str[p];
		Action action = LRTable.getAction(s, symbol);

		switch (action.act)
		{
			case Act::Shift: { // 移进
				states.push(action.state);
				symbols.push(symbol);
				p++; // 指向下一个字符
				break;
			}

			case Act::Reduce: { // 规约
				AttGrammer grammer = attGrammers[action.state - 1]; // 对应属性文法
				if (grammer.function) {
					grammer.function(this); // 执行对应语义动作
				}
				for (int i = 0; i < grammer.right.size(); i++) {
					states.pop();
					symbols.pop();
				}
				states.push( LRTable.getGoto(states.top(), grammer.left).state );
				symbols.push(grammer.left);
				break;
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
	for (auto quaternion : quaternions) {
		std::cout << quaternion.op << "  " << quaternion.arg1 << "  " << quaternion.arg2 << "  " << quaternion.result << std::endl;
	}
}