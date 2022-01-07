#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Global.h"

using namespace std;

class State {
public:
	vector<Action> actions;
	vector<Goto> gotos;
};

class LRAnalysisTable {
public:
	vector<State> table;
	vector<string> actionSymbols;
	vector<string> gotoSymbols;

	LRAnalysisTable();

	Action getAction(const int& state, const string& symbol);
	Goto getGoto(const int& state, const string& symbol);
};

Action LRAnalysisTable::getAction(const int& state, const string& symbol) {
	int i;
	i = indexOf(actionSymbols, split(symbol, string("_"))[0]); // �� _ �ָ�, ֧�ֽű�
	if (i != -1) {
		return table[state].actions[i];
	}
	else {
		return Action();
	}
}

Goto LRAnalysisTable::getGoto(const int& state, const string& symbol) {
	int i = indexOf(gotoSymbols, split(symbol, string("_"))[0]); // �� _ �ָ�, ֧�ֽű�
	if (i != -1) {
		return table[state].gotos[i];
	}
	else {
		return Goto();
	}
}

LRAnalysisTable::LRAnalysisTable() {
	int actionSymbolNum = 0;
	int gotoSymbolNum = 0;

	string lineStr, str;
	fstream LRFile("LRAnalysisTable.csv");
	if (!LRFile.is_open()) cout << "LR�������ļ���ʧ��!" << endl;

	// ��һ����action��goto��symbol
	getline(LRFile, lineStr);
	stringstream ss(lineStr);
	bool gotoFlag = false;
	while (getline(ss, str, ',')) {
		if (ifCapital(str)) gotoFlag = true; // �Ӵ�д��ĸ��ʼ, �϶�Ϊ���ս��
		if (!gotoFlag) { // ActionSymbol
			actionSymbols.push_back(str);
			actionSymbolNum++;
		}
		else { // GotoSymbol
			gotoSymbols.push_back(str);
			gotoSymbolNum++;
		}
	}

	// ʣ�µ���״̬
	string flag;
	int toState;
	while (getline(LRFile, lineStr)) {
		stringstream ss(lineStr);
		int symbolNum = 0;
		State state;
		while (getline(ss, str, ',')) {
			if (symbolNum < actionSymbolNum) { // Action
				if (str == "") {
					state.actions.push_back(Action(Act::Fail));
				}
				else if (str == "acc") {
					state.actions.push_back(Action(Act::Acc));
				}
				else {
					flag = split(str, "_")[0];
					toState = str2Number(split(str, "_")[1]);
					if (flag == "s")
						state.actions.push_back(Action(Act::Shift, toState));
					else if (flag == "r")
						state.actions.push_back(Action(Act::Reduce, toState));
				}
			}
			else if (symbolNum < actionSymbolNum + gotoSymbolNum) {
				if (str == "")
					state.gotos.push_back(Goto());
				else
					state.gotos.push_back(Goto(str2Number(str)));
			}
			else cout << "LR�����������������!" << endl;
			symbolNum++;
		}
		table.push_back(state);
	}
	LRFile.close();
}