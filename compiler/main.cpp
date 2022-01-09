
#include "Parser.h"
#include "LRAnalysisTable.h"


int Sid = 0;
int Bid = 0;
int Lid = 0;
int Mid = 0;
int Tid = 0;
int Eid = 0;
int Aid = 0;
int Gid = 0;

string S() {
	return "S_" + std::to_string(Sid);
}

string B() {
	return "B_" + std::to_string(Bid);
}

string L() {
	return "L_" + std::to_string(Lid);
}

string M() {
	return "M_" + std::to_string(Mid);
}

string T() {
	return "T_" + std::to_string(Tid);
}

string E() {
	return "E_" + std::to_string(Eid);
}

string A() {
	return "A_" + std::to_string(Aid);
}

string G() {
	return "G_" + std::to_string(Gid);
}

bool attGrammer0func(Parser* parser) {

	return true;
}

// S->while B do L
bool attGrammer1func(Parser* parser) { 
	Sid++;
	Attribute& attributeOfS = parser->symbolTabel.getAttribute(S());
	Attribute& attributeOfB = parser->symbolTabel.getAttribute(B());
	Attribute& attributeOfL = parser->symbolTabel.getAttribute(L());
	if (attributeOfL.nextList != -1) {
		parser->backPatch(attributeOfL.nextList, attributeOfB.quad);
	}
	
	parser->backPatch(attributeOfB.trueList, attributeOfL.quad);
	parser->emit("j", "-", "-", std::to_string(attributeOfB.quad));
	Bid--;
	Lid--;
	return true;
}

// B->MT
bool attGrammer2func(Parser* parser) { 
	Bid++;
	Attribute& attributeOfB = parser->symbolTabel.getAttribute(B()); 
	Attribute& attributeOfM = parser->symbolTabel.getAttribute(M()); 
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T());
	attributeOfB.quad = attributeOfM.quad;
	attributeOfB.trueList = attributeOfT.trueList;
	Mid--;
	Tid--;
	return true;
}

// L->MS
bool attGrammer3func(Parser* parser) { 
	Lid++;
	Attribute& attributeOfL = parser->symbolTabel.getAttribute(L());
	Attribute& attributeOfM = parser->symbolTabel.getAttribute(M()); 
	Attribute& attributeOfS = parser->symbolTabel.getAttribute(S());
	attributeOfL.quad = attributeOfM.quad;
	attributeOfL.nextList = attributeOfS.nextList;
	Mid--;
	Sid--;
	return true;
}

// M->' '
bool attGrammer4func(Parser* parser) {
	Mid++;
	Attribute& attributeOfM = parser->symbolTabel.getAttribute(M()); 
	attributeOfM.quad = parser->nextQuad;
	return true;
}

// E->id
bool attGrammer5func(Parser* parser) { 
	Eid++;
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); 
	Attribute& attributeOfId = parser->symbolTabel.getAttribute("id");
	attributeOfE.place = attributeOfId.place;
	return true;
}

// S->A
bool attGrammer6func(Parser* parser) { 
	Sid++;
	Attribute& attributeOfS = parser->symbolTabel.getAttribute(S());
//	attributeOfS.nextList = parser->makeList(parser->nextQuad);
	Attribute& attributeOfA = parser->symbolTabel.getAttribute(A());

	Aid--;
	return true;
}

// A->G=E
bool attGrammer7func(Parser* parser) {
	Aid++;
	Attribute& attributeOfG = parser->symbolTabel.getAttribute(G()); 
	Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); 
	parser->emit(":=", attributeOfE.place, " ", attributeOfG.place);
	Gid--;
	Eid--;
	return true;
}

// T->E1 relop E2
bool attGrammer8func(Parser* parser) {
	Tid++;
	Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); 
	Attribute& attributeOfE2 = parser->symbolTabel.getAttribute(E());
	Eid--;
	Attribute& attributeOfE1 = parser->symbolTabel.getAttribute(E());
	Attribute& attributeOfrelop = parser->symbolTabel.getAttribute("relop");
	attributeOfT.trueList = parser->makeList(parser->nextQuad);
	attributeOfT.falseList = parser->makeList(parser->nextQuad + 1);
	string a = attributeOfrelop.op;
	string b = "j";
	b = b + a;
	parser->emit(b, attributeOfE1.place, attributeOfE2.place, "0");
	parser->emit("j", "-", "-", "0");
	return true;
}

// G->id
bool attGrammer9func(Parser* parser) {
	Attribute& attributeOfId = parser->symbolTabel.getAttribute("id");
	if (attributeOfId.type != "Variable") return false; // G不是变量, 出错
	Gid++;
	Attribute& attributeOfG = parser->symbolTabel.getAttribute(G()); 
	attributeOfG.place = attributeOfId.place;
	return true;
}




int main() {
	
	Parser parser;
	// S-> S
	AttGrammer attGrammer0;
	attGrammer0.left = "S";
	attGrammer0.right.push_back("S");
	attGrammer0.function = attGrammer0func;
	parser.attGrammers.push_back(attGrammer0);


	// S->while B do L
	AttGrammer attGrammer1;
	attGrammer1.left = "S";
	attGrammer1.right.push_back("while");
	attGrammer1.right.push_back("B");
	attGrammer1.right.push_back("do");
	attGrammer1.right.push_back("L");
	attGrammer1.function = attGrammer1func;
	parser.attGrammers.push_back(attGrammer1);

	// B->MT
	AttGrammer attGrammer2;
	attGrammer2.left = "B";
	attGrammer2.right.push_back("M");
	attGrammer2.right.push_back("T");
	attGrammer2.function = attGrammer2func;
	parser.attGrammers.push_back(attGrammer2);

	// L->MS
	AttGrammer attGrammer3;
	attGrammer3.left = "L";
	attGrammer3.right.push_back("M");
	attGrammer3.right.push_back("S");
	attGrammer3.function = attGrammer3func;
	parser.attGrammers.push_back(attGrammer3);

	// M->' '
	AttGrammer attGrammer4;
	attGrammer4.left = "M";
	attGrammer4.right.push_back(" ");
	attGrammer4.function = attGrammer4func;
	parser.attGrammers.push_back(attGrammer4);

	// E->id
	AttGrammer attGrammer5;
	attGrammer5.left = "E";
	attGrammer5.right.push_back("id");
	attGrammer5.function = attGrammer5func;
	parser.attGrammers.push_back(attGrammer5);

	// S->A
	AttGrammer attGrammer6;
	attGrammer6.left = "S";
	attGrammer6.right.push_back("A");
	attGrammer6.function = attGrammer6func;
	parser.attGrammers.push_back(attGrammer6);

	// A->G=E
	AttGrammer attGrammer7;
	attGrammer7.left = "A";
	attGrammer7.right.push_back("G");
	attGrammer7.right.push_back("=");
	attGrammer7.right.push_back("E");
	attGrammer7.function = attGrammer7func;
	parser.attGrammers.push_back(attGrammer7);

	// T->E relop T
	AttGrammer attGrammer8;
	attGrammer8.left = "T";
	attGrammer8.right.push_back("E");
	attGrammer8.right.push_back("relop");
	attGrammer8.right.push_back("T");
	attGrammer8.function = attGrammer8func;
	parser.attGrammers.push_back(attGrammer8);



	// G->id
	AttGrammer attGrammer9;
	attGrammer9.left = "G";
	attGrammer9.right.push_back("id");
	attGrammer9.function = attGrammer9func;
	parser.attGrammers.push_back(attGrammer9);

	std::cout << "while  1 > 2 do  a = 5" << '\n';
	vector<string> str;
	str.push_back("while");
	str.push_back(" ");
	str.push_back("1");
	str.push_back(">");
	str.push_back("2");
	str.push_back("do");
	str.push_back(" ");
	str.push_back("a");
	str.push_back("=");
	str.push_back("5");
	str.push_back("#");

	parser.init(); // 初始化一下
	if (parser.parse(str)) parser.print();
	else std::cout << "语法出错!" << std::endl;

	std::cout<<'\n' << "while  1 > 2 do  while  3 < a do  b = 6" << '\n';
	vector<string> str1;
	str1.push_back("while");
	str1.push_back(" ");
	str1.push_back("1");
	str1.push_back(">");
	str1.push_back("2");
	str1.push_back("do");
	str1.push_back(" ");
	str1.push_back("while");
	str1.push_back(" ");
	str1.push_back("3");
	str1.push_back("<");
	str1.push_back("a");
	str1.push_back("do");
	str1.push_back(" ");
	str1.push_back("b");
	str1.push_back("=");
	str1.push_back("6");
	str1.push_back("#");

	parser.init(); // 初始化一下
	if (parser.parse(str1)) parser.print();
	else std::cout << "语法出错!" << std::endl;

	return 0;
	
}