
#include "Parser.h"
#include "LRAnalysisTable.h"

namespace assignment {

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

	void run() {

		Parser parser("assignmentLRTable.csv");

		// S'->S
		AttGrammer attGrammer1;
		attGrammer1.left = "S'";
		attGrammer1.right.push_back("S");
		attGrammer1.function = attGrammer1func;
		parser.attGrammers.push_back(attGrammer1);

		// S->G=E
		AttGrammer attGrammer2;
		attGrammer2.left = "S";
		attGrammer2.right.push_back("G");
		attGrammer2.right.push_back("=");
		attGrammer2.right.push_back("E");
		attGrammer2.function = attGrammer2func;
		parser.attGrammers.push_back(attGrammer2);

		// E->E+T
		AttGrammer attGrammer3;
		attGrammer3.left = "E";
		attGrammer3.right.push_back("E");
		attGrammer3.right.push_back("+");
		attGrammer3.right.push_back("T");
		attGrammer3.function = attGrammer3func;
		parser.attGrammers.push_back(attGrammer3);

		// E->E-T
		AttGrammer attGrammer4;
		attGrammer3.left = "E";
		attGrammer3.right.push_back("E");
		attGrammer3.right.push_back("-");
		attGrammer3.right.push_back("T");
		attGrammer4.function = attGrammer4func;
		parser.attGrammers.push_back(attGrammer4);

		// E->T
		AttGrammer attGrammer5;
		attGrammer5.left = "E";
		attGrammer5.right.push_back("T");
		attGrammer5.function = attGrammer5func;
		parser.attGrammers.push_back(attGrammer5);

		// T->T*F
		AttGrammer attGrammer6;
		attGrammer6.left = "T";
		attGrammer6.right.push_back("T");
		attGrammer6.right.push_back("*");
		attGrammer6.right.push_back("F");
		attGrammer6.function = attGrammer6func;
		parser.attGrammers.push_back(attGrammer6);

		// T->T/F
		AttGrammer attGrammer7;
		attGrammer7.left = "T";
		attGrammer7.right.push_back("T");
		attGrammer7.right.push_back("/");
		attGrammer7.right.push_back("F");
		attGrammer7.function = attGrammer7func;
		parser.attGrammers.push_back(attGrammer7);

		// T->F
		AttGrammer attGrammer8;
		attGrammer8.left = "T";
		attGrammer8.right.push_back("F");
		attGrammer8.function = attGrammer8func;
		parser.attGrammers.push_back(attGrammer8);

		// F->(E)
		AttGrammer attGrammer9;
		attGrammer9.left = "F";
		attGrammer9.right.push_back("(");
		attGrammer9.right.push_back("F");
		attGrammer9.right.push_back(")");
		attGrammer9.function = attGrammer9func;
		parser.attGrammers.push_back(attGrammer9);

		// F->id
		AttGrammer attGrammer10;
		attGrammer10.left = "F";
		attGrammer10.right.push_back("id");
		attGrammer10.function = attGrammer10func;
		parser.attGrammers.push_back(attGrammer10);

		// F->id
		AttGrammer attGrammer11;
		attGrammer11.left = "G";
		attGrammer11.right.push_back("id");
		attGrammer11.function = attGrammer11func;
		parser.attGrammers.push_back(attGrammer11);

		vector<string> str;
		string s;
		cout << "请输入赋值语句:\n";
		while (1) {
			cin >> s;
			str.push_back(s);
			if (s == string("#")) break;
		}

		parser.init(); // 初始化一下
		if (parser.parse(str)) parser.print();
		else std::cout << "语法出错!" << std::endl;

	}

}

namespace boolean {
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
		return true;
	}

	void run() {
		Parser parser("booleanLRTable");

		// S'->S
		AttGrammer attGrammer1;
		attGrammer1.left = "S'";
		attGrammer1.right.push_back("S");
		attGrammer1.function = attGrammer1func;
		parser.attGrammers.push_back(attGrammer1);

		// S->G=E
		AttGrammer attGrammer2;
		attGrammer2.left = "S";
		attGrammer2.right.push_back("G");
		attGrammer2.right.push_back("=");
		attGrammer2.right.push_back("E");
		attGrammer2.function = attGrammer2func;
		parser.attGrammers.push_back(attGrammer2);
	}
}
//条件语句
namespace conditional{

	int Eid = 0;
	int Fid = 0;
	int Tid = 0;
	int Hid = 0;
	int Mid = 0;
	int Nid = 0;
	string E() {
		return "E_" + std::to_string(Eid);
	}

	string F() {
		return "F_" + std::to_string(Fid);
	}

	string T() {
		return "T_" + std::to_string(Tid);
	}

	string H() {
		return "H_" + std::to_string(Hid);
	}
	string M() {
		return "M_" + std::to_string(Mid);
	}
	string N() {
		return "N_" + std::to_string(Nid);
	}
	// E'->E
	bool attGrammer1func(Parser* parser) { 
		return true;
	}

	// E->if T then [M] H
	bool attGrammer2func(Parser* parser) { 

		Eid++;
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
		Attribute& attributeOfH = parser->symbolTabel.getAttribute(H()); // 得到H的所有属性
		Tid--;
		Hid--;
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(M());
		Mid--;
		parser->pop();
		parser->backPatch(attributeOfT.trueList, attributeOfM.quad);
		attributeOfE.nextList = attributeOfT.falseList;
		parser->backPatch(attributeOfE.nextList, parser->nextQuad);

		return true;
	}

	// E->if T then [M] H [N] else [M] H  
	bool attGrammer3func(Parser* parser) { 
		Attribute& attributeOfM2 = parser->symbolTabel.getAttribute(M());
		Mid--;
		Attribute& attributeOfM1 = parser->symbolTabel.getAttribute(M());
		Mid--;

		Eid++;
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
		Tid--;
		Attribute& attributeOfH2 = parser->symbolTabel.getAttribute(H()); // 得到E的所有属性
		Hid--;
		Attribute& attributeOfH1 = parser->symbolTabel.getAttribute(H()); // 得到E的所有属性
		Hid--;
		parser->backPatch(attributeOfT.trueList, attributeOfM1.quad);
		parser->backPatch(attributeOfT.falseList, attributeOfM2.quad);
		Nid--;
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(N()); // 得到N的所有属性
		Nid--;
		attributeOfE.nextList = attributeOfN.nextList;
		parser->pop();
		parser->backPatch(attributeOfE.nextList, parser->nextQuad);
		return true;
	}

	// T->F relop F
	bool attGrammer4func(Parser* parser) { 
		Tid++;
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
		Attribute& attributeOfRelop = parser->symbolTabel.getAttribute("relop"); // 得到Relop的所有属性
		Attribute& attributeOfF2 = parser->symbolTabel.getAttribute(F()); // 得到E的所有属性
		Fid--;
		Attribute& attributeOfF1 = parser->symbolTabel.getAttribute(F()); // 得到T的所有属性
		Fid--;	
		attributeOfT.trueList = parser->makeList(parser->nextQuad);
		attributeOfT.falseList = parser->makeList(parser->nextQuad + 1);
		parser->emit("j" + attributeOfRelop.op, attributeOfF1.place, attributeOfF2.place, "0");
		parser->emit("j"," "," ","0");

		return true;
	}
	// F->id
	bool attGrammer5func(Parser* parser) {
		Fid++;
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
		attributeOfF.place = attributeOfId.place;
		return true;
	}
	// H ->G = F
	bool attGrammer6func(Parser* parser) { 
		Mid++;
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(M());
		attributeOfM.quad = parser->nextQuad;
		Hid++;
		Attribute& attributeOfH = parser->symbolTabel.getAttribute(H()); // 得到T的所有属性
		Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 得到T的所有属性
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到E的所有属性
		Fid--;
		parser->emit(":=", attributeOfF.place, " ", attributeOfG.place);
		Nid++;
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(N()); // 得到N的所有属性
		attributeOfN.nextList = parser->makeList(parser->nextQuad);
		parser->emit("j", " ", " ", "0");
		return true;
	}

	// G->id
	bool attGrammer7func(Parser* parser) {
		Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 得到F的所有属性
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
		if (attributeOfId.type != "Variable") return false; // G不是变量, 出错
		attributeOfG.place = attributeOfId.place;
		return true;
	}

	void run() {

		Parser parser("conditionalLRTable.csv");

		// E'->E
		AttGrammer attGrammer1;
		attGrammer1.left = "E'";
		attGrammer1.right.push_back("E");
		attGrammer1.function = attGrammer1func;
		parser.attGrammers.push_back(attGrammer1);

		// E->if T then H
		AttGrammer attGrammer2;
		attGrammer2.left = "E";
		attGrammer2.right.push_back("if");
		attGrammer2.right.push_back("T");
		attGrammer2.right.push_back("then");
		attGrammer2.right.push_back("H");
		attGrammer2.function = attGrammer2func;
		parser.attGrammers.push_back(attGrammer2);

		// E->if T then H else H 
		AttGrammer attGrammer3;
		attGrammer3.left = "E";
		attGrammer3.right.push_back("if");
		attGrammer3.right.push_back("T");
		attGrammer3.right.push_back("then");
		attGrammer3.right.push_back("H");
		attGrammer3.right.push_back("else");
		attGrammer3.right.push_back("H");
		attGrammer3.function = attGrammer3func;
		parser.attGrammers.push_back(attGrammer3);

		// T->F relop F
		AttGrammer attGrammer4;
		attGrammer4.left = "T";
		attGrammer4.right.push_back("F");
		attGrammer4.right.push_back("relop");
		attGrammer4.right.push_back("F");
		attGrammer4.function = attGrammer4func;
		parser.attGrammers.push_back(attGrammer4);

		// F->id
		AttGrammer attGrammer5;
		attGrammer5.left = "F";
		attGrammer5.right.push_back("id");
		attGrammer5.function = attGrammer5func;
		parser.attGrammers.push_back(attGrammer5);

		// H ->G = F
		AttGrammer attGrammer6;
		attGrammer6.left = "H";
		attGrammer6.right.push_back("G");
		attGrammer6.right.push_back("=");
		attGrammer6.right.push_back("F");
		attGrammer6.function = attGrammer6func;
		parser.attGrammers.push_back(attGrammer6);


		// G->id
		AttGrammer attGrammer7;
		attGrammer7.left = "G";
		attGrammer7.right.push_back("id");
		attGrammer7.function = attGrammer7func;
		parser.attGrammers.push_back(attGrammer7);

		vector<string> str;
		string s;
		cout << "请输入条件语句:\n";
		while (1) {
			cin >> s;
			str.push_back(s);
			if (s == string("#")) break;
		}
		parser.init(); // 初始化一下
		if (parser.parse(str)) parser.print();
		else std::cout << "语法出错!" << std::endl;

	}

}

int main() {
	assignment::run();
}
