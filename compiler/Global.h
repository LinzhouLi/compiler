#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <regex>

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
	string place;
	string op;

	Attribute() : type(""), trueList(-1), falseList(-1), nextList(-1), quad(-1), place(""), op("") { }
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

// 判断字符串是否为数字类型
bool isNumber(const string& str) {
	std::istringstream sin(str);
	double test;
	return sin >> test && sin.eof();
}

// 判断字符串是否为布尔型
bool isBool(const string& str) {
	return str == string("true") || str == string("false");
}

// 判断字符串是否为合法变量
bool ifVariable(const string& str) {
	std::regex r("^\\w+$");
	return std::regex_match(str, r);
}

// 判断字符串是否为大写字母
bool ifCapital(const string& str) {
	std::regex r("^[A-Z]$");
	return std::regex_match(str, r);
}

// 判断字符串是否为relop
bool ifRelop(const string& str) {
	if (str == "<" || str == ">" || str == "<=" || str == ">=" || str == "==")
		return true;
	else return false;
}

// 字符串转换为数字类型
double str2Number(const string& str) {
	std::istringstream sin(str);
	double test;
	sin >> test;
	return test;
}

// 字符串转换为布尔型
bool str2Bool(const string& str) {
	if (str == string("true")) {
		return true;
	}
	else return false;
}

// 字符串分割
vector<string> split(const string& str, const string& delimiter) {
	string::size_type lastPos = str.find_first_not_of(delimiter, 0);
	string::size_type pos = str.find_first_of(delimiter, lastPos);
	vector<string> tokens;
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiter, pos);
		pos = str.find_first_of(delimiter, lastPos);
	}
	return tokens;
}