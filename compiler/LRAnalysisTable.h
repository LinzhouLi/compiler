#pragma once

#include <vector>
#include <string>
#include "Global.h"

using std::vector;
using std::string;

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
	i = indexOf(actionSymbols, split(symbol, string("_"))[0]); // 用 _ 分割, 支持脚标
	if (i != -1) {
		return table[state].actions[i];
	}
	else {
		return Action();
	}
}

Goto LRAnalysisTable::getGoto(const int& state, const string& symbol) {
	int i = indexOf(gotoSymbols, split(symbol, string("_"))[0]); // 用 _ 分割, 支持脚标
	if (i != -1) {
		return table[state].gotos[i];
	}
	else {
		return Goto();
	}
}