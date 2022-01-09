
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
		Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Eid--;
		parser->emit(":=", attributeOfE.place, " ", attributeOfG.place);
		return true;
	}

	// E->E+T
	bool attGrammer3func(Parser* parser) {
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Tid--;
		string newTemp = parser->getNewTemp();
		parser->emit("+", attributeOfE.place, attributeOfT.place, newTemp);
		attributeOfE.place = newTemp;// E.place := newtemp;
		return true;
	}

	// E->E-T
	bool attGrammer4func(Parser* parser) {
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Tid--;
		string newTemp = parser->getNewTemp();
		parser->emit("-", attributeOfE.place, attributeOfT.place, newTemp);
		attributeOfE.place = newTemp;// E.place := newtemp;
		return true;
	}

	// E->T
	bool attGrammer5func(Parser* parser) {
		Eid++;
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Tid--;
		attributeOfE.place = attributeOfT.place;
		return true;
	}

	// T->T*F
	bool attGrammer6func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		Fid--;
		string newTemp = parser->getNewTemp();
		parser->emit("*", attributeOfT.place, attributeOfF.place, newTemp);
		attributeOfT.place = newTemp;// T.place := newtemp;
		return true;
	}

	// T->T/F
	bool attGrammer7func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		if (attributeOfF.place == "0")  // 鐩存帴闄や互0閿欒
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
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		Fid--;
		attributeOfT.place = attributeOfF.place;
		return true;
		return true;
	}

	// F->(E)
	bool attGrammer9func(Parser* parser) {
		Fid++;
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(F()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Eid--;
		attributeOfT.place = attributeOfE.place;
		return true;
	}

	// F->id
	bool attGrammer10func(Parser* parser) {
		Fid++;
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 寰楀埌Number鐨勬墍鏈夊睘鎬?
		attributeOfF.place = attributeOfId.place;
		return true;
	}

	// G->id
	bool attGrammer11func(Parser* parser) {
		Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 寰楀埌Number鐨勬墍鏈夊睘鎬?
		if (attributeOfId.type != "Variable") return false; // G涓嶆槸鍙橀噺, 鍑洪敊
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
		attGrammer4.left = "E";
		attGrammer4.right.push_back("E");
		attGrammer4.right.push_back("-");
		attGrammer4.right.push_back("T");
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
		attGrammer9.right.push_back("E");
		attGrammer9.right.push_back(")");
		attGrammer9.function = attGrammer9func;
		parser.attGrammers.push_back(attGrammer9);

		// F->id
		AttGrammer attGrammer10;
		attGrammer10.left = "F";
		attGrammer10.right.push_back("id");
		attGrammer10.function = attGrammer10func;
		parser.attGrammers.push_back(attGrammer10);

		// G->id
		AttGrammer attGrammer11;
		attGrammer11.left = "G";
		attGrammer11.right.push_back("id");
		attGrammer11.function = attGrammer11func;
		parser.attGrammers.push_back(attGrammer11);

		vector<string> str;
		string s;
		cout << "璇疯緭鍏ヨ祴鍊艰鍙?\n";
		while (1) {
			cin >> s;
			str.push_back(s);
			if (s == string("#")) break;
		}

		parser.init(); // 鍒濆鍖栦竴涓?
		if (parser.parse(str)) parser.print();
		else std::cout << "璇硶鍑洪敊!" << std::endl;

	}

}

namespace compose {

	const vector<string> NonTerminators = {
		"E", "F", "G", "M", "N", "P", "Q", "S", "T"
	};

	vector<int> NonTerminatorID(NonTerminators.size(), 0);

	string get(const string& symbol, const string& method) {
		string result = symbol;
		for (int i = 0; i < NonTerminators.size(); i++) {
			if (NonTerminators[i] == symbol) {
				if (method == "left") {
					NonTerminatorID[i]++;
					result += "_" + std::to_string(NonTerminatorID[i]);
				}
				else if (method == "right") {
					result += "_" + std::to_string(NonTerminatorID[i]);
					NonTerminatorID[i]--;
				}
				else if (method == "both") {
					result += "_" + std::to_string(NonTerminatorID[i]);
				}
				break;
			}
		}
		return result;
	}
	

	// S'->S
	bool attGrammer1func(Parser* parser) {
		return true;
	}

	// S->while M1 (P){M2 S N}
	bool attGrammer2func(Parser* parser) {
		Attribute& attributeOfS = parser->symbolTabel.getAttribute(get("S", "both"));
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "right"));
		Attribute& attributeOfM2 = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfM1 = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(get("N", "right"));
		parser->backPatch(attributeOfP.trueList, attributeOfM2.quad);
		parser->backPatch(attributeOfS.nextList, attributeOfN.quad);
		parser->backPatch(attributeOfN.nextList, attributeOfM1.quad);
		attributeOfS.nextList = attributeOfP.falseList;
		return true;
	}

	// S->if(P){M S N}
	bool attGrammer3func(Parser* parser) {
		Attribute& attributeOfS = parser->symbolTabel.getAttribute(get("S", "both"));
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "right"));
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(get("N", "right"));
		parser->backPatch(attributeOfP.trueList, attributeOfM.quad);
		attributeOfS.nextList = parser->merge(attributeOfP.falseList, attributeOfN.nextList);
		return true;
	}

	// S->if(P){ M1 S1 } N else{ M2 S2 }
	bool attGrammer4func(Parser* parser) {
		Attribute& attributeOfS2 = parser->symbolTabel.getAttribute(get("S", "right"));
		Attribute& attributeOfS1 = parser->symbolTabel.getAttribute(get("S", "both"));
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "right"));
		Attribute& attributeOfM2 = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfM1 = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(get("N", "right"));
		parser->backPatch(attributeOfP.trueList, attributeOfM1.quad);
		parser->backPatch(attributeOfP.falseList, attributeOfM2.quad);
		return true;
	}

	// S->G=E;
	bool attGrammer5func(Parser* parser) {
		Attribute& attributeOfS = parser->symbolTabel.getAttribute(get("S", "left"));
		Attribute& attributeOfG = parser->symbolTabel.getAttribute(get("G", "right"));
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(get("E", "right"));
		string variableName = attributeOfG.place;
		Attribute& attributeOfVariable = parser->variableTable.getAttribute(variableName);
		attributeOfVariable.type = attributeOfE.type;
		parser->emit(":=", attributeOfE.place, " ", variableName);
		attributeOfS.nextList = parser->makeList(parser->nextQuad);
		return true;
	}

	// E->E+T
	bool attGrammer6func(Parser* parser) {
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(get("E", "both"));
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(get("T", "right"));
		if (attributeOfE.type != "Number" || attributeOfT.type != "Number") {
			cout << "类型错误" << endl;
			return false;
		}
		string newTemp = parser->getNewTemp();
		parser->emit("+", attributeOfE.place, attributeOfT.place, newTemp);
		attributeOfE.place = newTemp;// T.place := newtemp;
		attributeOfE.type = "Number";
		return true;
	}

	// E->E-T
	bool attGrammer7func(Parser* parser) {
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(get("E", "both"));
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(get("T", "right"));
		if (attributeOfE.type != "Number" || attributeOfT.type != "Number") {
			cout << "类型错误" << endl;
			return false;
		}
		string newTemp = parser->getNewTemp();
		parser->emit("-", attributeOfE.place, attributeOfT.place, newTemp);
		attributeOfE.place = newTemp;// T.place := newtemp;
		attributeOfE.type = "Number";
		return true;
	}

	// E->T
	bool attGrammer8func(Parser* parser) {
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(get("E", "left"));
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(get("T", "right"));
		attributeOfE.place = attributeOfT.place;
		attributeOfE.type = attributeOfT.type;
		return true;
	}

	// T->T*F
	bool attGrammer9func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(get("T", "both"));
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(get("F", "right"));
		if (attributeOfT.type != "Number" || attributeOfF.type != "Number") {
			cout << "类型错误" << endl;
			return false;
		}
		string newTemp = parser->getNewTemp();
		parser->emit("*", attributeOfT.place, attributeOfF.place, newTemp);
		attributeOfT.place = newTemp;// T.place := newtemp;
		attributeOfT.type = "Number";
		return true;
	}

	// T->T/F
	bool attGrammer10func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(get("T", "both"));
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(get("F", "right"));
		if (attributeOfT.type != "Number" || attributeOfF.type != "Number") {
			cout << "类型错误" << endl;
			return false;
		}
		if (attributeOfF.type == "Number" && attributeOfF.place == "0") {
			cout << "直接除以0错误" << endl;
			return false;
		}
		string newTemp = parser->getNewTemp();
		parser->emit("/", attributeOfT.place, attributeOfF.place, newTemp);
		attributeOfT.place = newTemp;// T.place := newtemp;
		attributeOfT.type = "Number";
		return true;
	}

	// T->F
	bool attGrammer11func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(get("T", "left"));
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(get("F", "right"));
		attributeOfT.place = attributeOfF.place;
		attributeOfT.type = attributeOfF.type;
		return true;
	}

	// F->(E)
	bool attGrammer12func(Parser* parser) {
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(get("F", "left"));
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(get("E", "right"));
		attributeOfF.place = attributeOfE.place;
		attributeOfF.type = attributeOfE.type;
		return true;
	}

	// F->id
	bool attGrammer13func(Parser* parser) {
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(get("F", "left"));
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id");
		if (attributeOfId.type == "Variable") {
			string variableName = attributeOfId.place;
			Attribute& attributeOfVariable = parser->variableTable.getAttribute(variableName);
			if (attributeOfVariable.type == "Undefined") {
				cout << "变量" << variableName << "未定义" << endl;
				return false;
			}
		}
		attributeOfF.place = attributeOfId.place;
		attributeOfF.type = attributeOfId.type;
		return true;
	}

	// G->id
	bool attGrammer14func(Parser* parser) {
		Attribute& attributeOfG = parser->symbolTabel.getAttribute(get("G", "left"));
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id");
		if (attributeOfId.type != "Variable") {
			cout << "赋值号左边应为变量, 但得到了" << attributeOfId.type << "类型" << endl;
			return false;
		}
		attributeOfG.place = attributeOfId.place;
		attributeOfG.type = attributeOfId.type;
		return true;
	}

	// P->P1 || M P2
	bool attGrammer15func(Parser* parser) {
		Attribute& attributeOfP2 = parser->symbolTabel.getAttribute(get("P", "right"));
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfP1 = parser->symbolTabel.getAttribute(get("P", "both"));
		parser->backPatch(attributeOfP1.falseList, attributeOfM.quad);
		attributeOfP1.trueList = parser->merge(attributeOfP1.trueList, attributeOfP2.trueList);
		attributeOfP1.falseList = attributeOfP2.falseList;
		return true;
	}

	// P->P1 && M P2
	bool attGrammer16func(Parser* parser) {
		Attribute& attributeOfP2 = parser->symbolTabel.getAttribute(get("P", "right"));
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(get("M", "right"));
		Attribute& attributeOfP1 = parser->symbolTabel.getAttribute(get("P", "both"));
		parser->backPatch(attributeOfP1.trueList, attributeOfM.quad);
		attributeOfP1.trueList = attributeOfP2.trueList;
		attributeOfP1.falseList = parser->merge(attributeOfP1.falseList, attributeOfP2.falseList);
		return true;
	}

	// P->!P
	bool attGrammer17func(Parser * parser) {
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "both"));
		swap(attributeOfP.trueList, attributeOfP.falseList);
		return true;
	}


	// P->(P)
	bool attGrammer18func(Parser * parser) {
		return true;
	}

	// P->E relop E
	bool attGrammer19func(Parser* parser) {
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "left"));
		Attribute& attributeOfRelop = parser->symbolTabel.getAttribute("relop");
		Attribute& attributeOfE2 = parser->symbolTabel.getAttribute(get("E", "right"));
		Attribute& attributeOfE1 = parser->symbolTabel.getAttribute(get("E", "right"));
		attributeOfP.type = "Bool";
		attributeOfP.trueList = parser->makeList(parser->nextQuad);
		attributeOfP.falseList = parser->makeList(parser->nextQuad + 1);
		parser->emit("j" + attributeOfRelop.op, attributeOfE1.place, attributeOfE2.place, "0");
		parser->emit("j", " ", " ", "0");
		return true;
	}

	// S->G=P
	bool attGrammer20func(Parser* parser) {
		Attribute& attributeOfS = parser->symbolTabel.getAttribute(get("S", "left"));
		Attribute& attributeOfG = parser->symbolTabel.getAttribute(get("G", "right"));
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "right"));
		string variableName = attributeOfG.place;
		Attribute& attributeOfVariable = parser->variableTable.getAttribute(variableName);
		attributeOfVariable.type = "Bool";
		parser->backPatch(attributeOfP.trueList, parser->nextQuad);
		parser->backPatch(attributeOfP.falseList, parser->nextQuad + 1);
		parser->emit(":=", "true", " ", variableName);
		parser->emit(":=", "false", " ", variableName);
		attributeOfS.nextList = parser->makeList(parser->nextQuad);
		return true;
	}

	// P->id
	bool attGrammer21func(Parser* parser) {
		Attribute& attributeOfP = parser->symbolTabel.getAttribute(get("P", "left"));
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id");
		if (attributeOfId.type == "Variable") {
			string variableName = attributeOfId.place;
			Attribute& attributeOfVariable = parser->variableTable.getAttribute(variableName);
			if (attributeOfVariable.type == "Undefined") {
				cout << "变量" << variableName << "未定义" << endl;
				return false;
			}
			else if (attributeOfVariable.type != "Bool") {
				cout << "变量" << variableName << "类型为" << attributeOfVariable.type << ", 应该为Bool" << endl;
				return false;
			}
		}
		else if (attributeOfId.type != "Bool") {
			cout << attributeOfId.place << "类型为" << attributeOfId.type << ", 应该为Bool" << endl;
			return false;
		}
		attributeOfP.place = attributeOfId.place;
		attributeOfP.type = attributeOfId.type;
		return true;
	}

	// M->e
	bool attGrammer22func(Parser* parser) {
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(get("M", "left"));
		attributeOfM.quad = parser->nextQuad;
		return true;
	}

	// N->e
	bool attGrammer23func(Parser* parser) {
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(get("N", "left"));
		attributeOfN.nextList = parser->makeList(parser->nextQuad);
		attributeOfN.quad = parser->nextQuad;
		parser->emit("j", " ", " ", "0");
		return true;
	}

	// S->S1 S2
	bool attGrammer24func(Parser* parser) {
		Attribute& attributeOfS2 = parser->symbolTabel.getAttribute(get("S", "right"));
		Attribute& attributeOfS1 = parser->symbolTabel.getAttribute(get("S", "both"));
		return true;
	}
	
	void run() {

		Parser parser("composeLRTable.csv");

		// S'->S
		AttGrammer attGrammer1;
		attGrammer1.left = "S'";
		attGrammer1.right.push_back("S");
		attGrammer1.function = attGrammer1func;
		parser.attGrammers.push_back(attGrammer1);

		// S->whileM(P){MS}
		AttGrammer attGrammer2;
		attGrammer2.left = "S'";
		attGrammer2.right.push_back("while");
		attGrammer2.right.push_back("M");
		attGrammer2.right.push_back("(");
		attGrammer2.right.push_back("P");
		attGrammer2.right.push_back(")");
		attGrammer2.right.push_back("{");
		attGrammer2.right.push_back("M");
		attGrammer2.right.push_back("S");
		attGrammer2.right.push_back("}");
		attGrammer2.function = attGrammer2func;
		parser.attGrammers.push_back(attGrammer2);

		// S->if(P){MS}
		AttGrammer attGrammer3;
		attGrammer3.left = "S";
		attGrammer3.right.push_back("if");
		attGrammer3.right.push_back("(");
		attGrammer3.right.push_back("P");
		attGrammer3.right.push_back(")");
		attGrammer3.right.push_back("{");
		attGrammer3.right.push_back("M");
		attGrammer3.right.push_back("S");
		attGrammer3.right.push_back("}");
		attGrammer3.function = attGrammer3func;
		parser.attGrammers.push_back(attGrammer3);

		// S->if(P){MS}Nelse{MS}
		AttGrammer attGrammer4;
		attGrammer4.left = "S";
		attGrammer4.right.push_back("if");
		attGrammer4.right.push_back("(");
		attGrammer4.right.push_back("P");
		attGrammer4.right.push_back(")");
		attGrammer4.right.push_back("{");
		attGrammer4.right.push_back("M");
		attGrammer4.right.push_back("S");
		attGrammer4.right.push_back("}");
		attGrammer4.right.push_back("N");
		attGrammer4.right.push_back("else");
		attGrammer4.right.push_back("{");
		attGrammer4.right.push_back("M");
		attGrammer4.right.push_back("S");
		attGrammer4.right.push_back("}");
		attGrammer4.function = attGrammer4func;
		parser.attGrammers.push_back(attGrammer4);

		// S->G=E;
		AttGrammer attGrammer5;
		attGrammer5.left = "S";
		attGrammer5.right.push_back("G");
		attGrammer5.right.push_back("=");
		attGrammer5.right.push_back("E");
		attGrammer5.right.push_back(";");
		attGrammer5.function = attGrammer5func;
		parser.attGrammers.push_back(attGrammer5);

		// E->E+T
		AttGrammer attGrammer6;
		attGrammer6.left = "E";
		attGrammer6.right.push_back("E");
		attGrammer6.right.push_back("+");
		attGrammer6.right.push_back("T");
		attGrammer6.function = attGrammer6func;
		parser.attGrammers.push_back(attGrammer6);

		// E->E-T
		AttGrammer attGrammer7;
		attGrammer7.left = "E";
		attGrammer7.right.push_back("E");
		attGrammer7.right.push_back("-");
		attGrammer7.right.push_back("T");
		attGrammer7.function = attGrammer7func;
		parser.attGrammers.push_back(attGrammer7);

		// E->T
		AttGrammer attGrammer8;
		attGrammer8.left = "E";
		attGrammer8.right.push_back("T");
		attGrammer8.function = attGrammer8func;
		parser.attGrammers.push_back(attGrammer8);

		// T->T*F
		AttGrammer attGrammer9;
		attGrammer9.left = "T";
		attGrammer9.right.push_back("T");
		attGrammer9.right.push_back("*");
		attGrammer9.right.push_back("F");
		attGrammer9.function = attGrammer9func;
		parser.attGrammers.push_back(attGrammer9);

		// T->T/F
		AttGrammer attGrammer10;
		attGrammer10.left = "T";
		attGrammer10.right.push_back("T");
		attGrammer10.right.push_back("/");
		attGrammer10.right.push_back("F");
		attGrammer10.function = attGrammer10func;
		parser.attGrammers.push_back(attGrammer10);

		// T->F
		AttGrammer attGrammer11;
		attGrammer11.left = "T";
		attGrammer11.right.push_back("F");
		attGrammer11.function = attGrammer11func;
		parser.attGrammers.push_back(attGrammer11);

		// F->(E)
		AttGrammer attGrammer12;
		attGrammer12.left = "F";
		attGrammer12.right.push_back("(");
		attGrammer12.right.push_back("E");
		attGrammer12.right.push_back(")");
		attGrammer12.function = attGrammer12func;
		parser.attGrammers.push_back(attGrammer12);

		// F->id
		AttGrammer attGrammer13;
		attGrammer13.left = "F";
		attGrammer13.right.push_back("id");
		attGrammer13.function = attGrammer13func;
		parser.attGrammers.push_back(attGrammer13);

		// G->id
		AttGrammer attGrammer14;
		attGrammer14.left = "G";
		attGrammer14.right.push_back("id");
		attGrammer14.function = attGrammer14func;
		parser.attGrammers.push_back(attGrammer14);

		// P->P||MP
		AttGrammer attGrammer15;
		attGrammer15.left = "P";
		attGrammer15.right.push_back("P");
		attGrammer15.right.push_back("||");
		attGrammer15.right.push_back("M");
		attGrammer15.right.push_back("P");
		attGrammer15.function = attGrammer15func;
		parser.attGrammers.push_back(attGrammer15);

		// P->P&&MP
		AttGrammer attGrammer16;
		attGrammer16.left = "P";
		attGrammer16.right.push_back("P");
		attGrammer16.right.push_back("&&");
		attGrammer16.right.push_back("M");
		attGrammer16.right.push_back("P");
		attGrammer16.function = attGrammer16func;
		parser.attGrammers.push_back(attGrammer16);

		// P->!P
		AttGrammer attGrammer17;
		attGrammer17.left = "P";
		attGrammer17.right.push_back("!");
		attGrammer17.right.push_back("P");
		attGrammer17.function = attGrammer17func;
		parser.attGrammers.push_back(attGrammer17);

		// P->(P)
		AttGrammer attGrammer18;
		attGrammer18.left = "P";
		attGrammer18.right.push_back("(");
		attGrammer18.right.push_back("P");
		attGrammer18.right.push_back(")");
		attGrammer18.function = attGrammer18func;
		parser.attGrammers.push_back(attGrammer18);

		// P->E relop E
		AttGrammer attGrammer19;
		attGrammer19.left = "P";
		attGrammer19.right.push_back("E");
		attGrammer19.right.push_back("relop");
		attGrammer19.right.push_back("E");
		attGrammer19.function = attGrammer19func;
		parser.attGrammers.push_back(attGrammer19);

		// S->G=P
		AttGrammer attGrammer20;
		attGrammer20.left = "S";
		attGrammer20.right.push_back("G");
		attGrammer20.right.push_back("=");
		attGrammer20.right.push_back("P");
		attGrammer20.right.push_back(";");
		attGrammer20.function = attGrammer20func;
		parser.attGrammers.push_back(attGrammer20);

		// P->id
		AttGrammer attGrammer21;
		attGrammer21.left = "P";
		attGrammer21.right.push_back("id");
		attGrammer21.function = attGrammer21func;
		parser.attGrammers.push_back(attGrammer21);

		// M->epsilon
		AttGrammer attGrammer22;
		attGrammer22.left = "M";
		attGrammer22.right.push_back("epsilon");
		attGrammer22.function = attGrammer22func;
		parser.attGrammers.push_back(attGrammer22);

		// N->epsilon
		AttGrammer attGrammer23;
		attGrammer23.left = "N";
		attGrammer23.right.push_back("epsilon");
		attGrammer23.function = attGrammer23func;
		parser.attGrammers.push_back(attGrammer23);

		// S->SS
		AttGrammer attGrammer24;
		attGrammer24.left = "S";
		attGrammer24.right.push_back("S");
		attGrammer24.right.push_back("S");
		attGrammer24.function = attGrammer24func;
		parser.attGrammers.push_back(attGrammer24);
		

		vector<string> str;
		string s;
		bool flag = true;
		cout << "请输入语句:\n";
		while (flag) {
			while (1) {
				cin >> s;
				if (s == string("#")) {
					str.push_back(s);
					flag = false;
					break;
				}
				else if (s == string("{") || s == string("while") || 
					s == string("||") || s == string("&&")) {
					str.push_back(s);
					str.push_back("epsilon");
				}
				else if (s == string("}")) {
					str.push_back("epsilon");
					str.push_back(s);
				}
				else str.push_back(s);
			}
		}

		parser.init(); // 初始化一下
		if (parser.parse(str)) parser.print();
		else std::cout << "语法出错!" << std::endl;

	}
}

namespace boolean {
	using namespace std;
                              //&&	||	!	true false(	  )	  i	 rop   # 
           int action[18][10]={ { 0	, 0	 ,4	,5	, 6	 ,7	, 0	, 8	, 0	,  0},//0
				{ 9	, 0	 ,0	,0	, 0	 ,0	,-2	, 0	, 0	,100},//1
				{-2	,10	 ,0	,0	, 0	 ,0	,-2	, 0	, 0	, -2},//2
				{-4	,-4	 ,0	,0	, 0	 ,0	,-4	, 0	, 0	, -4},//3
				{ 0	, 0	 ,4	,5	, 6	 ,7	, 0	, 8	, 0	,  0},//4
				{-7	,-7	 ,0	,0	, 0	 ,0	,-7	, 0	, 0	, -7},//5
				{-8	,-8	 ,0	,0	, 0	 ,0	,-8	, 0	, 0	, -8},//6
				{ 0	, 0	 ,4	,5	, 6	 ,7	, 0	, 8	, 0	,  0},//7
				{ 0	, 0	 ,0	,0	, 0	 ,0	, 0	, 0	,13	,  0},//8
				{ 0	, 0	 ,4	,5	, 6	 ,7	, 0	, 8	, 0	,  0},//9
				{ 0	, 0	 ,4	,5	, 6	 ,7	, 0	, 8	, 0	,  0},//10
				{-5	,-5	 ,0	,0	, 0	 ,0	,-5	, 0	, 0	, -5},//11
				{ 9	, 0	 ,0	,0	, 0	 ,0	,16	, 0	, 0	,  0},//12
				{ 0	, 0	 ,0	,0	, 0	 ,0	, 0	,17	, 0	,  0},//13
				{-1	,10  ,0	,0	, 0	 ,0	,-1	, 0	, 0	, -1},//14
				{-3	,-3	 ,0	,0	, 0	 ,0	,-3	, 0	, 0	, -3},//15
				{-6	,-6	 ,0	,0	, 0	 ,0	,-6	, 0	, 0	, -6},//16
				{-9	,-9	 ,0	,0	, 0	 ,0	,-9	, 0	, 0	, -9}};//17
 
                               //B   T    F
           int gotol[18][3]={   {1	,2	, 3},//0
				{0	,0	, 0},//1
				{0	,0	, 0},//2
				{0	,0	, 0},//3
				{0	,0	,11},//4
				{0	,0	, 0},//5
				{0	,0	, 0},//6
				{12	,2	, 3},//7
				{0	,0	, 0},//8
				{0	,14	, 3},//9
                                {0	,0	,15},//10
				{0	,0	, 0},//11
				{0	,0	, 0},//12
                                {0	,0	, 0},//13
				{0	,0	, 0},//14
				{0	,0	, 0},//15
				{0	,0	, 0},//16
                                {0	,0	, 0}};//17
 
	//终结符集合
	string endls[10]={"&&","||","!","true","false", "(",")", "i","rop","#" };
	//非终结符集合
	string noends[3]={"B","T","F"};
	//产生式集合
	string products[10]={"B","B && T", "T","T || F","F","! F","( B )","true", "false","i rop i"};
	//栈类
	class statestack
	{	
		private:
			int *base;//栈底指针
			int *top;//栈顶指针
			int size;//栈内元素个数
			int stacksize;//栈的大小
		public:
			statestack()
			{
				size=0;
				stacksize=20;
				base=new int[stacksize];;
				top=base;	
			}
			int getTop()//获取栈顶的元素。
			{
				if(base==top)
				{
					return -1;  
				}
				else
				{
					return *(top-1);
				}
			}  
			bool statePush(int elem)//元素入栈
			{
				++size;
				(*top)=elem;
				++top;
				return true;
			}
			void statePop(int time)//元素出栈
			{
				for(int i=0;i<time;i++)
				{
					--top;
					--size;
				}
			}
			void printState()//输出栈内的所有元素
			{
				string str=" ";
				int *pre;
				for(pre=base;pre<top;pre++)
				{
					if(*pre>9)
					{
						char ch1=(*pre/10)+48;
						char ch2=(*pre%10)+48;
				str+=ch1;
						str+=ch2;
					}
					else
					{
			    char ch=*pre+48;
					    str+=ch;
					}		
				}
				cout<<setw(15)<<setiosflags(ios_base::left)<<str;
			}
	};

	class symbolstack
	{
		private:
			string *base;
			string *top;
			int size;
			int stacksize;
		public:
			symbolstack()
			{
				size=0;
				stacksize=20;
				base=new string[stacksize];;
				top=base;
			}	
			string getTop()//获取栈顶的元素。
			{
				if(base==top)
				{
					return " ";  
				}
				else
				{
					return *(top-1);
				}
			}  
			//元素入栈
			bool symbolPush(string elem)
			{
				++size;
				*top=elem;
				++top;
				return true;
			}
			//元素出栈
			void symbolPop(int time)
			{
				for(int i=0;i<time;i++)
				{
					--top;
					--size;
				}
			}
			//输出栈内的所有元素
			void printSymbol()
			{
				string str=" ";
				string *pre;
				for(pre=base;pre<top;pre++)
				{
				str+=*pre;
				}
				cout<<setw(15)<<setiosflags(ios_base::left)<<str;
			}
	};

	class analyseLR
	{
	private:
		int step;
		string inputstr;//布尔表达式
		statestack state;//状态栈
		symbolstack symbol;//符号栈
		vector<string> fors;//四元式
	public:
		//构造函数
		analyseLR()
		{	
			step=0;
			inputstr=" ";
			state=statestack();
			symbol=symbolstack();
		}
		//初始化两个栈
		void initstack()
		{
			state.statePush(0);
			symbol.symbolPush("#");
		}
	    void printInfo(string str)
		{
			cout<<str<<endl;
		}
		void printInfoEX(string str1,string str2,string str3,string str4,string str5)
		{
			cout<<setw(6)<<setiosflags(ios_base::left)<<str1;
			cout<<setw(15)<<setiosflags(ios_base::left)<<str2;
			cout<<setw(15)<<setiosflags(ios_base::left)<<str3;
			cout<<setw(24)<<setiosflags(ios_base::left)<<str4;
			cout<<setw(10)<<setiosflags(ios_base::left)<<str5<<endl;
		}
		void printfInfoEX(int str1,statestack state,symbolstack symbol,string str4,string str5)
		{
			cout<<setw(5)<<setiosflags(ios_base::left)<<str1;
			state.printState();
			symbol.printSymbol();
			cout<<setw(25)<<setiosflags(ios_base::left)<<str4;
			cout<<setw(10)<<setiosflags(ios_base::left)<<str5<<endl;
		}

		void initstageInfo()
		{
			printInfo("布尔表达式的文法如下：");
			cout<<"(0)  B’-> B"<<endl;
			cout<<"(1)  B -> B && T"<<endl;
			cout<<"(2)  B -> T"<<endl;
			cout<<"(3)  T -> T || F"<<endl;
			cout<<"(4)  T -> F"<<endl;
			cout<<"(5)  F -> ! F"<<endl;
			cout<<"(6)  F -> ( B )"<<endl;
			cout<<"(7)  F -> true"<<endl;
			cout<<"(8)  F -> false "<<endl;
			cout<<"(9)  F -> i rop i"<<endl;
		}

		void initInputstr()
		{
			printInfo("请输入布尔表达式:");
			getline(cin,inputstr);
		}
		//将终结符和非终结符转换为action和gotol表中对应的列号
		int strtoInt(string str)
		{
			if(str=="&&")
				return 0;
			if(str=="||")
				return 1;
			if(str=="!")
				return 2;
			if(str=="true")
				return 3;
			if(str=="false")
				return 4;
			if(str=="(")
				return 5;
			if(str==")")
				return 6;
			if(str=="i")
				return 7;
			if(str=="rop"||str=="<"||str==">")
				return 8;
			if(str=="#")
				return 9;
			if(str=="B")
				return 0;
			if(str=="T")
				return 1;
			if(str=="F")
				return 2;
		}
		//判断字符是终结符还是非终结符
		bool judgeSymbol(string str)
		{
			for (int i = 0; i < 10; i++)
			{
				if (endls[i] == str||str=="<"||str==">")
					return true;
			}
			return false;
		}
		//根据产生式选择非终结符
		int chooseNoEnds(int num)
		{
			if(num==1||num==2)
				return 0;//选择“B”
			if(num==3||num==4)
				return 1;//选择“T”
		    return 2;//选择“F”
		}
		//计算字符串中元素的个数
		int calculatenum(string str)
		{
			int num=0;
			for(int i=0;i<str.size();++i)
			{
				if(isgraph(str[i]))
					continue;
				else
				{
					++num;
				}
			}
			++num;
		return num;
		}	

		vector<string> separatestrEX(string str)
		{
			vector<string> vec;
			int pos=0;
			for(int i=0;i<str.size();++i)
			{   
				if(isspace(str[i]))//如果当前字符是空格
				{
					vec.push_back(str.substr(pos,i-pos));//复制起始位置为pos且长度为i-pos的字符串。
					pos=i+1;
				}
			}
			vec.push_back(str.substr(pos,str.size()-pos));
			return vec;
		}

		string linkVectorstr(vector<string> &vecs,vector<string>::iterator iter)
		{
			string str=" ";
			vector<string>::iterator it;
				it=iter;
			for(it;it<vecs.end();it++)
			{
				//cout<<*it; 
				str+=*it;
			}
			return str;
		}

		void createforchief(int num, string lenstr, string ch)
		{
			//cout << num << "@" << lenstr<< "@" << ch << endl;
			vector<string> strs = separatestrEX(lenstr);
			vector<string>::iterator iter = strs.begin();
			string str = " ";
			string l1 = "(";
			string l2 = ")";
			string comma = ",";
			if (num == 1)
			{
				string l0 = "=";
				string arg1 = *iter;
				str = l1 + l0 + comma + arg1 + comma + "_" + comma + ch + l2;
			}
			if (num == 2)
			{
				string op = *iter;
				string arg1 = *(iter + 1);
				str = l1 + op + comma + arg1 + comma + "_" + comma + ch + l2;
			}
			if (num == 3)
			{
				string arg1 = *iter;
				string op = *(iter + 1);
				string arg2 = *(iter + 2);
				if (arg1 == "(")
				{
					str = l1 + "=" + comma + op + comma + "_" + comma + ch + l2;
				}
				else
				{
					str = l1 + op + comma + arg1 + comma + arg2 + comma + ch + l2;
				}
			}
			fors.push_back(str);
		}

		void printForsInfo()
		{
			printInfo("中间代码四元式为");
			vector<string>::iterator it=fors.begin();
			for(it;it<fors.end();it++)
			{
				cout<<*it<<endl;
			}
		}
		void Start()//开始函数.
		{
			initstageInfo();
			initstack();//初始化两个栈
			initInputstr();
			vector<string> vec=separatestrEX(inputstr);//分开布尔表达式
			vector<string>::iterator iter=vec.begin();
			//printInfo("LR分析法的过程如下");
			//printInfoEX("步骤","状态栈","符号栈","输入串","操作");


			for(iter;iter!=vec.end();++iter)//依次遍历字符
			{                                                            
				string str1=linkVectorstr(vec,iter);
				actionGoto(*iter,str1);
			}

			printForsInfo();
		}
		//LR分析函数
		void actionGoto(string str,string str1)
		{      		
			    int x=state.getTop();//当前栈顶状态
				int y=strtoInt(str);//当前将要输入的字符
				if(action[x][y]>0&&judgeSymbol(str)&&(str!="#"))//移进
				{
					//printfInfoEX(step,state,symbol,str1,"移入"+str);
					state.statePush(action[x][y]);
					symbol.symbolPush(str);	
					++step;
				}
				if(action[x][y]<0&&judgeSymbol(str))//规约
				{
					int operation=-action[x][y];
					string lenstr=products[operation];//需要规约的产生式


					int size=calculatenum(lenstr);//计算产生式的长度，进行规约
					int c=chooseNoEnds(operation);
					string ch=noends[c];//生成规约后的非终结符
					//生成四元式
					createforchief(size,lenstr,ch);

					//printfInfoEX(step,state,symbol,str1,"规约"+noends[c]+"->"+products[operation]);
					state.statePop(size);
					symbol.symbolPop(size);
					int topState=state.getTop();//获取规约后的栈顶状态
					if(gotol[topState][c]>0)
					{
						int g=gotol[topState][c];
						state.statePush(g);
						symbol.symbolPush(ch);
					}
			++step; 
					actionGoto(str,str1);
				}
				if((action[x][y]==100)&&(str=="#"))
				{
					//printfInfoEX(step,state,symbol,str1,"分析完成");
				}
			}
		};    

	void run() {
		analyseLR ana=analyseLR();
		ana.Start();	
	}
}

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
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfH = parser->symbolTabel.getAttribute(H()); // 寰楀埌H鐨勬墍鏈夊睘鎬?
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
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Tid--;
		Attribute& attributeOfH2 = parser->symbolTabel.getAttribute(H()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Hid--;
		Attribute& attributeOfH1 = parser->symbolTabel.getAttribute(H()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Hid--;
		parser->backPatch(attributeOfT.trueList, attributeOfM1.quad);
		parser->backPatch(attributeOfT.falseList, attributeOfM2.quad);
		Nid--;
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(N()); // 寰楀埌N鐨勬墍鏈夊睘鎬?
		Nid--;
		attributeOfE.nextList = attributeOfN.nextList;
		parser->pop();
		parser->backPatch(attributeOfE.nextList, parser->nextQuad);
		return true;
	}

	// T->F relop F
	bool attGrammer4func(Parser* parser) { 
		Tid++;
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfRelop = parser->symbolTabel.getAttribute("relop"); // 寰楀埌Relop鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfF2 = parser->symbolTabel.getAttribute(F()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Fid--;
		Attribute& attributeOfF1 = parser->symbolTabel.getAttribute(F()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
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
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 寰楀埌Number鐨勬墍鏈夊睘鎬?
		attributeOfF.place = attributeOfId.place;
		return true;
	}
	// H ->G = F
	bool attGrammer6func(Parser* parser) { 
		Mid++;
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(M());
		attributeOfM.quad = parser->nextQuad;
		Hid++;
		Attribute& attributeOfH = parser->symbolTabel.getAttribute(H()); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 寰楀埌T鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 寰楀埌E鐨勬墍鏈夊睘鎬?
		Fid--;
		parser->emit(":=", attributeOfF.place, " ", attributeOfG.place);
		Nid++;
		Attribute& attributeOfN = parser->symbolTabel.getAttribute(N()); // 寰楀埌N鐨勬墍鏈夊睘鎬?
		attributeOfN.nextList = parser->makeList(parser->nextQuad);
		parser->emit("j", " ", " ", "0");
		return true;
	}

	// G->id
	bool attGrammer7func(Parser* parser) {
		Attribute& attributeOfG = parser->symbolTabel.getAttribute("G"); // 寰楀埌F鐨勬墍鏈夊睘鎬?
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 寰楀埌Number鐨勬墍鏈夊睘鎬?
		if (attributeOfId.type != "Variable") return false; // G涓嶆槸鍙橀噺, 鍑洪敊
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
		cout << "璇疯緭鍏ユ潯浠惰鍙?\n";
		while (1) {
			cin >> s;
			str.push_back(s);
			if (s == string("#")) break;
		}
		parser.init(); // 鍒濆鍖栦竴涓?
		if (parser.parse(str)) parser.print();
		else std::cout << "璇硶鍑洪敊!" << std::endl;

	}

}
namespace loop{
	
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
	void run() {

		Parser parser("looplLRTable.csv");

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

		vector<string> str;
		string s;
		cout << "璇疯緭鍏ユ潯浠惰鍙?\n";
		while (1) {
			cin >> s;
			str.push_back(s);
			if (s == string("#")) break;
		}
		parser.init(); // 鍒濆鍖栦竴涓?
		if (parser.parse(str)) parser.print();
		else std::cout << "璇硶鍑洪敊!" << std::endl;

	}

}

int main() {
}
