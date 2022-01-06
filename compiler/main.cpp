
#include "Parser.h"
#include "LRAnalysisTable.h"


int Eid = 0;
int Fid = 0;
int Tid = 0;

string E() {
	return "E_" + std::to_string(Eid);
}

string F() {
	return "F_" + std::to_string(Fid);
}

string T() {
	return "T_" + std::to_string(Tid);
}

bool attGrammer1func(Parser* parser) { // E->E+T
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Tid--;
	string newTemp = parser->getNewTemp();
	parser->emit("+", attributeOfE.place, attributeOfT.place, newTemp);
	attributeOfE.place = newTemp;// E.place := newtemp;
	return true;
}

bool attGrammer2func(Parser* parser) { // E->T
	Eid++;
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Tid--;
	attributeOfE.place = attributeOfT.place;
	return true;
}

bool attGrammer3func(Parser* parser) { // T->T*F
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	Fid--;
	string newTemp = parser->getNewTemp();
	parser->emit("*", attributeOfT.place, attributeOfF.place, newTemp);
	attributeOfT.place = newTemp;// T.place := newtemp;
	return true;
}

bool attGrammer4func(Parser* parser) { // T->F
	Tid++;
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	Fid--;
	attributeOfT.place = attributeOfF.place;
	return true;
}

bool attGrammer5func(Parser* parser) { // F->(E)
	Fid++;
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(F()); // 得到T的所有属性
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Eid--;
	attributeOfT.place = attributeOfE.place;
	return true;
}

bool attGrammer6func(Parser* parser) { // F->id
	Fid++;
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
	attributeOfF.place = attributeOfId.place;
	return true;
}

int main() {
	
	Parser parser;

	// E->E+T
	AttGrammer attGrammer1;
	attGrammer1.left = "E";
	attGrammer1.right.push_back("E");
	attGrammer1.right.push_back("+");
	attGrammer1.right.push_back("T");
	attGrammer1.function = attGrammer1func;
	parser.attGrammers.push_back(attGrammer1);

	// E->T
	AttGrammer attGrammer2;
	attGrammer2.left = "E";
	attGrammer2.right.push_back("T");
	attGrammer2.function = attGrammer2func;
	parser.attGrammers.push_back(attGrammer2);

	// T->T*F
	AttGrammer attGrammer3;
	attGrammer3.left = "T";
	attGrammer3.right.push_back("T");
	attGrammer3.right.push_back("*");
	attGrammer3.right.push_back("F");
	attGrammer3.function = attGrammer3func;
	parser.attGrammers.push_back(attGrammer3);

	// T->F
	AttGrammer attGrammer4;
	attGrammer4.left = "T";
	attGrammer4.right.push_back("F");
	attGrammer4.function = attGrammer4func;
	parser.attGrammers.push_back(attGrammer4);

	// F->(E)
	AttGrammer attGrammer5;
	attGrammer5.left = "F";
	attGrammer5.right.push_back("(");
	attGrammer5.right.push_back("E");
	attGrammer5.right.push_back(")");
	attGrammer5.function = attGrammer5func;
	parser.attGrammers.push_back(attGrammer5);

	// F->Number
	AttGrammer attGrammer6;
	attGrammer6.left = "F";
	attGrammer6.right.push_back("id");
	attGrammer6.function = attGrammer6func;
	parser.attGrammers.push_back(attGrammer6);

	vector<string> str;
	str.push_back("123");
	str.push_back("*");
	str.push_back("(");
	str.push_back("-345");
	str.push_back("+");
	str.push_back("457.22");
	str.push_back(")");
	str.push_back("#");

	parser.init(); // 初始化一下
	if (parser.parse(str)) parser.print();
	else std::cout << "语法出错!" << std::endl;
	return 0;
	
}