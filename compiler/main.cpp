
#include "Parser.h"
#include "LRAnalysisTable.h"
#include "sstream"
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

// S'->S
bool attGrammer1func(Parser* parser) { 
	return true;
}

// S->G=E
bool attGrammer2func(Parser* parser) { 
	Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 得到T的所有属性
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Eid--;
	parser->emit(":=", attributeOfE.place, " ", attributeOfG.place);
	return true;
}

// E->E+T
bool attGrammer3func(Parser* parser) { 
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Tid--;
	string newTemp = parser->getNewTemp();
	parser->emit("+", attributeOfE.place, attributeOfT.place, newTemp);
	attributeOfE.place = newTemp;// E.place := newtemp;
	return true;
}

// E->E-T
bool attGrammer4func(Parser* parser) { 
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Tid--;
	string newTemp = parser->getNewTemp();
	parser->emit("-", attributeOfE.place, attributeOfT.place, newTemp);
	attributeOfE.place = newTemp;// E.place := newtemp;
	return true;
}

// E->T
bool attGrammer5func(Parser* parser) { 
	Eid++;
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Tid--;
	attributeOfE.place = attributeOfT.place;
	return true;
}

// T->T*F
bool attGrammer6func(Parser* parser) { 
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	Fid--;
	string newTemp = parser->getNewTemp();
	parser->emit("*", attributeOfT.place, attributeOfF.place, newTemp);
	attributeOfT.place = newTemp;// T.place := newtemp;
	return true;
}

// T->T/F
bool attGrammer7func(Parser* parser) {
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	if (attributeOfF.place == "0")  // 直接除以0错误
		return false;
	Fid--;
	string newTemp = parser->getNewTemp();
	parser->emit("/", attributeOfT.place, attributeOfF.place, newTemp);
	attributeOfT.place = newTemp;// T.place := newtemp;
	return true;
}

// T->F
bool attGrammer8func(Parser* parser) {
	Tid++;
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	Fid--;
	attributeOfT.place = attributeOfF.place;
	return true;
	return true;
}

// F->(E)
bool attGrammer9func(Parser* parser) {
	Fid++;
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(F()); // 得到T的所有属性
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
	Eid--;
	attributeOfT.place = attributeOfE.place;
	return true;
}

// F->id
bool attGrammer10func(Parser* parser) {
	Fid++;
	Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
	Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
	attributeOfF.place = attributeOfId.place;
	return true;
}

// G->id
bool attGrammer11func(Parser* parser) {
	Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 得到F的所有属性
	Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
	if (attributeOfId.type != "Variable") return false; // G不是变量, 出错
	attributeOfG.place = attributeOfId.place;
	return true;
}
//S->goto id
bool attGrammer12func(Parser* parser) {
	Attribute& attributeOfL = parser->symbolTabel.getAttribute("L"); 
	Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
	parser->emit("j", "-", "-", attributeOfId.place);
	return true;
}

int main() {
	
	Parser parser;
	Parser parser_goto;
	// S'->S
	AttGrammer attGrammer1;
	attGrammer1.left = "S'";
	attGrammer1.right.push_back("S");
	attGrammer1.function = attGrammer1func;
	parser.attGrammers.push_back(attGrammer1);
	parser_goto.attGrammers.push_back(attGrammer1);
	// S->G=E
	AttGrammer attGrammer2;
	attGrammer2.left = "S";
	attGrammer2.right.push_back("G");
	attGrammer2.right.push_back("=");
	attGrammer2.right.push_back("E");
	attGrammer2.function = attGrammer2func;
	parser.attGrammers.push_back(attGrammer2);
	parser_goto.attGrammers.push_back(attGrammer2);
	// E->E+T
	AttGrammer attGrammer3;
	attGrammer3.left = "E";
	attGrammer3.right.push_back("E");
	attGrammer3.right.push_back("+");
	attGrammer3.right.push_back("T");
	attGrammer3.function = attGrammer3func;
	parser.attGrammers.push_back(attGrammer3);
	parser_goto.attGrammers.push_back(attGrammer3);
	// E->E-T
	AttGrammer attGrammer4;
	attGrammer4.left = "E";
	attGrammer4.right.push_back("E");
	attGrammer4.right.push_back("-");
	attGrammer4.right.push_back("T");
	attGrammer4.function = attGrammer4func;
	parser.attGrammers.push_back(attGrammer4);
	parser_goto.attGrammers.push_back(attGrammer4);
	// E->T
	AttGrammer attGrammer5;
	attGrammer5.left = "E";
	attGrammer5.right.push_back("T");
	attGrammer5.function = attGrammer5func;
	parser.attGrammers.push_back(attGrammer5);
	parser_goto.attGrammers.push_back(attGrammer5);
	// T->T*F
	AttGrammer attGrammer6;
	attGrammer6.left = "T";
	attGrammer6.right.push_back("T");
	attGrammer6.right.push_back("*");
	attGrammer6.right.push_back("F");
	attGrammer6.function = attGrammer6func;
	parser.attGrammers.push_back(attGrammer6);
	parser_goto.attGrammers.push_back(attGrammer6);
	// T->T/F
	AttGrammer attGrammer7;
	attGrammer7.left = "T";
	attGrammer7.right.push_back("T");
	attGrammer7.right.push_back("/");
	attGrammer7.right.push_back("F");
	attGrammer7.function = attGrammer7func;
	parser.attGrammers.push_back(attGrammer7);
	parser_goto.attGrammers.push_back(attGrammer7);
	// T->F
	AttGrammer attGrammer8;
	attGrammer8.left = "T";
	attGrammer8.right.push_back("F");
	attGrammer8.function = attGrammer8func;
	parser.attGrammers.push_back(attGrammer8);
	parser_goto.attGrammers.push_back(attGrammer8);
	// F->(E)
	AttGrammer attGrammer9;
	attGrammer9.left = "F";
	attGrammer9.right.push_back("(");
	attGrammer9.right.push_back("F");
	attGrammer9.right.push_back(")");
	attGrammer9.function = attGrammer9func;
	parser.attGrammers.push_back(attGrammer9);
	parser_goto.attGrammers.push_back(attGrammer9);
	// F->id
	AttGrammer attGrammer10;
	attGrammer10.left = "F";
	attGrammer10.right.push_back("id");
	attGrammer10.function = attGrammer10func;
	parser.attGrammers.push_back(attGrammer10);
	parser_goto.attGrammers.push_back(attGrammer10);
	// F->id
	AttGrammer attGrammer11;
	attGrammer11.left = "G";
	attGrammer11.right.push_back("id");
	attGrammer11.function = attGrammer11func;
	parser.attGrammers.push_back(attGrammer11);
	parser_goto.attGrammers.push_back(attGrammer11);
	//S->goto id
	AttGrammer attGrammer12;
	attGrammer12.left = "S";
	attGrammer12.right.push_back("goto");
	attGrammer12.right.push_back("id");
	attGrammer12.function = attGrammer12func;
	parser.attGrammers.push_back(attGrammer12);
	parser_goto.attGrammers.push_back(attGrammer12);
	

	vector<string> str;
	str.push_back("a");
	str.push_back("=");
	str.push_back("1");
	str.push_back("+");
	str.push_back("(");
	str.push_back("5");
	str.push_back("*");
	str.push_back("3");
	str.push_back("+");
	str.push_back("1");
	str.push_back(")");
	str.push_back("#");

	parser.init(); // 初始化一下
	if (parser.parse(str)) parser.print ();
	else std::cout << "语法出错!" << std::endl;



	vector<string> str_goto;
	str_goto.push_back("goto");
	str_goto.push_back("3");    //前面的赋值语句执行完可以在这里执行goto语句    跳转到哪一步在这里改
	str_goto.push_back("#");




	parser_goto.init(); // 初始化一下
	if (parser_goto.parse(str_goto)) parser_goto.print();
	else std::cout << "语法出错!" << std::endl;
	int n;
	stringstream stream;
	stream << *(str_goto.begin() + 1);
	stream >> n;
	parser.print_line(n);
	return 0;
	
}