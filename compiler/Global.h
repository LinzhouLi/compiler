#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <regex>

using std::string;
using std::vector;

enum class Act { // Action����
	Shift, Reduce, Acc, Fail // �ƽ�, ��Լ, ����, ����
};

class Action { // LR���е�Action��
public:
	Act act;
	int state;

	Action(const Act& _act = Act::Fail, const int& _state = -1) : act(_act), state(_state) { }
};

class Goto { // LR���е�Goto��
public:
	int state;

	Goto(const int& _state = -1) : state(_state) { }
};

class Quaternion { // ��Ԫʽ
public:
	string op;
	string arg1;
	string arg2;
	string result;

	Quaternion(const string& _op = "/", const string& _arg1 = "/", const string& _arg2 = "/", const string& _result = "/")
		: op(_op), arg1(_arg1), arg2(_arg2), result(_result) { }
};

class Attribute { // ���ŵ�����
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

// �ж��ַ����Ƿ�Ϊ��������
bool isNumber(const string& str) {
	std::istringstream sin(str);
	double test;
	return sin >> test && sin.eof();
}

// �ж��ַ����Ƿ�Ϊ������
bool isBool(const string& str) {
	return str == string("true") || str == string("false");
}

// �ж��ַ����Ƿ�Ϊ�Ϸ�����
bool ifVariable(const string& str) {
	std::regex r("^\\w+$");
	return std::regex_match(str, r);
}

// �ж��ַ����Ƿ�Ϊ��д��ĸ
bool ifCapital(const string& str) {
	std::regex r("^[A-Z]$");
	return std::regex_match(str, r);
}

// �ж��ַ����Ƿ�Ϊrelop
bool ifRelop(const string& str) {
	if (str == "<" || str == ">" || str == "<=" || str == ">=" || str == "==")
		return true;
	else return false;
}

// �ַ���ת��Ϊ��������
double str2Number(const string& str) {
	std::istringstream sin(str);
	double test;
	sin >> test;
	return test;
}

// �ַ���ת��Ϊ������
bool str2Bool(const string& str) {
	if (str == string("true")) {
		return true;
	}
	else return false;
}

// �ַ����ָ�
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