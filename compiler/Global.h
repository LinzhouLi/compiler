#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;

enum class Act { // Action种类
	Shift, Reduce, Acc, Fail // 移进, 规约, 接受, 出错
};

class Action { // LR表中的Action项
public:
	Act act;
	int state;

	Action(const Act& _act = Act::Fail, const int& _state = -1) : act(_act), state(_state) { }
};

class Goto { // LR表中的Goto项
public:
	int state;

	Goto(const int& _state = -1) : state(_state) { }
};

class Quaternion { // 四元式
public:
	string op;
	string arg1;
	string arg2;
	string result;

	Quaternion(const string& _op = "/", const string& _arg1 = "/", const string& _arg2 = "/", const string& _result = "/")
		: op(_op), arg1(_arg1), arg2(_arg2), result(_result) { }
};

class Attribute { // 符号的属性
public:
	string type;
	int trueList;
	int falseList;
	int nextList;
	int quad;
	int place;
	string op;

	Attribute() : type(""), trueList(-1), falseList(-1), nextList(-1), quad(-1), place(0), op("") { }
};


/*
* Helper Function
*/
template <class T>
int indexOf(const vector<T>& vec, const T& item) {
	const T* p = vec.data();
	for (int i = 0; i < vec.size(); i++) {
		if (*(p + i) == item) {
			return i;
		}
	}
	return -1;
}