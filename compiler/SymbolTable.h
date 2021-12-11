#pragma once

#include <vector>
#include <string>
#include "Global.h"

using std::vector;
using std::string;

class SymbolTable { // ·ûºÅ±í
private:
	vector<Attribute> attributes;
	vector<string> strs;

public:
	Attribute& getAttribute(const string& s);
};

Attribute& SymbolTable::getAttribute(const string& s) {
	int p = indexOf(strs, s);
	if (p == -1) {
		strs.push_back(s);
		attributes.push_back(Attribute());
		p = attributes.size() - 1;
	}
	return attributes[p];
}