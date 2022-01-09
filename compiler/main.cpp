
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
	int Eid = 0;
	int Fid = 0;
	int Tid = 0;
	int Mid = 0;
	int Gid = 0;
	int Sid = 0;

	string S() {
		return "S_" + std::to_string(Sid);
	}

	string G() {
		return "G_" + std::to_string(Gid);
	}

	string E() {
		return "E_" + std::to_string(Eid);
	}

	string F() {
		return "F_" + std::to_string(Fid);
	}

	string T() {
		return "T_" + std::to_string(Tid);
	}

	string M() {
		return "M_" + std::to_string(Mid);
	}

	// S'->S
	bool attGrammer1func(Parser* parser) {
		return true;
	}

	// S->G=E
	bool attGrammer2func(Parser* parser) {
		Sid++;
		Attribute& attributeOfS = parser->symbolTabel.getAttribute(S());
		Attribute& attributeOfG = parser->symbolTabel.getAttribute(G()); // 得到T的所有属性
		Gid--;
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
		Eid--;
		parser->backPatch(attributeOfE.trueList, parser->nextQuad);
		parser->backPatch(attributeOfE.falseList, parser->nextQuad + 1);
		parser->emit(":=", "true", " ", attributeOfG.place);
		parser->emit(":=", "false", " ", attributeOfG.place);
		return true;
	}

	//E→E1 or M E2
	//{ backpatch(E1.falselist, M.quad);
	// E.truelist: = merge(E1.truelist, E2.truelist);
	// E.falselist: = E2.falselist }
	// E→E||MT
	bool attGrammer3func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到E的所有属性
		Tid--;
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(M()); // 得到M的所有属性
		Mid--;
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
		parser->backPatch(attributeOfE.falseList, attributeOfM.quad);
		attributeOfE.trueList = parser->merge(attributeOfE.trueList, attributeOfT.trueList);
		attributeOfE.falseList = attributeOfT.falseList;
		return true;
	}
	//E→E1and M E2
	//{ backpatch(E1.truelist, M.quad);
	// E.truelist: = E2.truelist;
	// E.falselist: = merge(E1.falselist,E2.falselist) }
	// E->E&&MT
	bool attGrammer4func(Parser* parser) {

		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
		Tid--;
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(M()); // 得到M的所有属性
		Mid--;

		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
		parser->backPatch(attributeOfE.trueList, attributeOfM.quad);
		attributeOfE.trueList = attributeOfT.trueList;
		attributeOfE.falseList = parser->merge(attributeOfE.falseList, attributeOfT.falseList);
		return true;

		/*
		string newTemp = parser->getNewTemp();
		parser->emit("-", attributeOfE.place, attributeOfT.place, newTemp);
		attributeOfE.place = newTemp;// E.place := newtemp;
		return true;
		*/
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

	// T->TrelopF
	bool attGrammer6func(Parser* parser) {
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性

		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
		Fid--;

		Attribute& attributeOfRelop = parser->symbolTabel.getAttribute("relop");
		attributeOfT.trueList = parser->makeList(parser->nextQuad);
		attributeOfT.falseList = parser->makeList((parser->nextQuad) + 1);
		parser->emit("j" + attributeOfRelop.op, attributeOfT.place, attributeOfF.place, "0");
		parser->emit("j", " ", " ", "0");
		return true;
	}
	// T->F
	bool attGrammer7func(Parser* parser) {
		Tid++;
		Attribute& attributeOfT = parser->symbolTabel.getAttribute(T()); // 得到T的所有属性
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到F的所有属性
		Fid--;
		attributeOfT.place = attributeOfF.place;
		return true;
	}

	// F->(E)
	bool attGrammer8func(Parser* parser) {
		Fid++;
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到T的所有属性
		Attribute& attributeOfE = parser->symbolTabel.getAttribute(E()); // 得到E的所有属性
		Eid--;
		attributeOfF.place = attributeOfE.place;
		return true;
	}

	// F->!F
	bool attGrammer9func(Parser* parser) {
		Attribute& attributeOfF = parser->symbolTabel.getAttribute(F()); // 得到T的所有属性
		int temp = attributeOfF.trueList;
		attributeOfF.trueList = attributeOfF.falseList;
		attributeOfF.falseList = temp;
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
		Gid++;
		Attribute& attributeOfG = parser->symbolTabel.getAttribute(G()); // 得到F的所有属性
		Attribute& attributeOfId = parser->symbolTabel.getAttribute("id"); // 得到Number的所有属性
		if (attributeOfId.type != "Variable") return false; // G不是变量, 出错
		attributeOfG.place = attributeOfId.place;
		return true;
	}

	// M->epsilon
	bool attGrammer12func(Parser* parser) {
		Mid++;
		Attribute& attributeOfM = parser->symbolTabel.getAttribute(M());
		attributeOfM.quad = parser->nextQuad;
		return true;
	}

	void run() {
		Parser parser("booleanLRTable.csv");

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

		// E->E||MT
		AttGrammer attGrammer3;
		attGrammer3.left = "E";
		attGrammer3.right.push_back("E");
		attGrammer3.right.push_back("||");
		attGrammer3.right.push_back("M");
		attGrammer3.right.push_back("T");
		attGrammer3.function = attGrammer3func;
		parser.attGrammers.push_back(attGrammer3);

		// E->E&&MT
		AttGrammer attGrammer4;
		attGrammer4.left = "E";
		attGrammer4.right.push_back("E");
		attGrammer4.right.push_back("&&");
		attGrammer4.right.push_back("M");
		attGrammer4.right.push_back("T");
		attGrammer4.function = attGrammer4func;
		parser.attGrammers.push_back(attGrammer4);

		// E->T
		AttGrammer attGrammer5;
		attGrammer5.left = "E";
		attGrammer5.right.push_back("T");
		attGrammer5.function = attGrammer5func;
		parser.attGrammers.push_back(attGrammer5);

		// T->TrelopF
		AttGrammer attGrammer6;
		attGrammer6.left = "T";
		attGrammer6.right.push_back("T");
		attGrammer6.right.push_back("relop");
		attGrammer6.right.push_back("F");
		attGrammer6.function = attGrammer6func;
		parser.attGrammers.push_back(attGrammer6);


		// T->F
		AttGrammer attGrammer7;
		attGrammer7.left = "T";
		attGrammer7.right.push_back("F");
		attGrammer7.function = attGrammer7func;
		parser.attGrammers.push_back(attGrammer7);

		// F->(E)
		AttGrammer attGrammer8;
		attGrammer8.left = "F";
		attGrammer8.right.push_back("(");
		attGrammer8.right.push_back("E");
		attGrammer8.right.push_back(")");
		attGrammer8.function = attGrammer8func;
		parser.attGrammers.push_back(attGrammer8);

		// F->!F
		AttGrammer attGrammer9;
		attGrammer9.left = "F";
		attGrammer9.right.push_back("!");
		attGrammer9.right.push_back("F");
		attGrammer9.function = attGrammer9func;
		parser.attGrammers.push_back(attGrammer9);

		// F->id
		AttGrammer attGrammer10;
		attGrammer10.left = "F";
		attGrammer10.right.push_back("id");
		attGrammer10.function = attGrammer10func;
		parser.attGrammers.push_back(attGrammer10);

		//G->id
		AttGrammer attGrammer11;
		attGrammer11.left = "G";
		attGrammer11.right.push_back("id");
		attGrammer11.function = attGrammer11func;
		parser.attGrammers.push_back(attGrammer11);

		//M->epsilon
		AttGrammer attGrammer12;
		attGrammer12.left = "M";
		attGrammer12.right.push_back("epsilon");
		attGrammer12.function = attGrammer12func;
		parser.attGrammers.push_back(attGrammer12);

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
				else if (s == string("{")) {
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
