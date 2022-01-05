#include "Parser.h"

void attGrammer1func(Parser* parser) {
	// 某文法的语义动作
	return;
}

Parser::Parser() {
	// 初始化LR分析表
	LRTable.actionSymbols.push_back("Number");
	LRTable.actionSymbols.push_back("+");
	LRTable.actionSymbols.push_back("*");
	LRTable.actionSymbols.push_back("(");
	LRTable.actionSymbols.push_back(")");
	LRTable.actionSymbols.push_back("#");

	LRTable.gotoSymbols.push_back("E");
	LRTable.gotoSymbols.push_back("T");
	LRTable.gotoSymbols.push_back("F");

	State state0;
	state0.actions.push_back(Action(Act::Shift, 5));
	state0.actions.push_back(Action(Act::Fail));
	state0.actions.push_back(Action(Act::Fail));
	state0.actions.push_back(Action(Act::Shift, 4));
	state0.actions.push_back(Action(Act::Fail));
	state0.actions.push_back(Action(Act::Fail));
	state0.gotos.push_back(Goto(1));
	state0.gotos.push_back(Goto(2));
	state0.gotos.push_back(Goto(3));
	LRTable.table.push_back(state0);

	State state1;
	state1.actions.push_back(Action(Act::Fail));
	state1.actions.push_back(Action(Act::Shift, 6));
	state1.actions.push_back(Action(Act::Fail));
	state1.actions.push_back(Action(Act::Fail));
	state1.actions.push_back(Action(Act::Fail));
	state1.actions.push_back(Action(Act::Acc));
	state1.gotos.push_back(Goto());
	state1.gotos.push_back(Goto());
	state1.gotos.push_back(Goto());
	LRTable.table.push_back(state1);

	State state2;
	state2.actions.push_back(Action(Act::Fail));
	state2.actions.push_back(Action(Act::Reduce, 2));
	state2.actions.push_back(Action(Act::Shift, 7));
	state2.actions.push_back(Action(Act::Fail));
	state2.actions.push_back(Action(Act::Reduce, 2));
	state2.actions.push_back(Action(Act::Reduce, 2));
	state2.gotos.push_back(Goto());
	state2.gotos.push_back(Goto());
	state2.gotos.push_back(Goto());
	LRTable.table.push_back(state2);

	State state3;
	state3.actions.push_back(Action(Act::Fail));
	state3.actions.push_back(Action(Act::Reduce, 4));
	state3.actions.push_back(Action(Act::Reduce, 4));
	state3.actions.push_back(Action(Act::Fail));
	state3.actions.push_back(Action(Act::Reduce, 4));
	state3.actions.push_back(Action(Act::Reduce, 4));
	state3.gotos.push_back(Goto());
	state3.gotos.push_back(Goto());
	state3.gotos.push_back(Goto());
	LRTable.table.push_back(state3);

	State state4;
	state4.actions.push_back(Action(Act::Shift, 5));
	state4.actions.push_back(Action(Act::Fail));
	state4.actions.push_back(Action(Act::Fail));
	state4.actions.push_back(Action(Act::Shift, 4));
	state4.actions.push_back(Action(Act::Fail));
	state4.actions.push_back(Action(Act::Fail));
	state4.gotos.push_back(Goto(8));
	state4.gotos.push_back(Goto(2));
	state4.gotos.push_back(Goto(3));
	LRTable.table.push_back(state4);

	State state5;
	state5.actions.push_back(Action(Act::Fail));
	state5.actions.push_back(Action(Act::Reduce, 6));
	state5.actions.push_back(Action(Act::Reduce, 6));
	state5.actions.push_back(Action(Act::Fail));
	state5.actions.push_back(Action(Act::Reduce, 6));
	state5.actions.push_back(Action(Act::Reduce, 6));
	state5.gotos.push_back(Goto());
	state5.gotos.push_back(Goto());
	state5.gotos.push_back(Goto());
	LRTable.table.push_back(state5);

	State state6;
	state6.actions.push_back(Action(Act::Shift, 5));
	state6.actions.push_back(Action(Act::Fail));
	state6.actions.push_back(Action(Act::Fail));
	state6.actions.push_back(Action(Act::Shift, 4));
	state6.actions.push_back(Action(Act::Fail));
	state6.actions.push_back(Action(Act::Fail));
	state6.gotos.push_back(Goto());
	state6.gotos.push_back(Goto(9));
	state6.gotos.push_back(Goto(3));
	LRTable.table.push_back(state6);

	State state7;
	state7.actions.push_back(Action(Act::Shift, 5));
	state7.actions.push_back(Action(Act::Fail));
	state7.actions.push_back(Action(Act::Fail));
	state7.actions.push_back(Action(Act::Shift, 4));
	state7.actions.push_back(Action(Act::Fail));
	state7.actions.push_back(Action(Act::Fail));
	state7.gotos.push_back(Goto());
	state7.gotos.push_back(Goto());
	state7.gotos.push_back(Goto(10));
	LRTable.table.push_back(state7);

	State state8;
	state8.actions.push_back(Action(Act::Fail));
	state8.actions.push_back(Action(Act::Shift, 6));
	state8.actions.push_back(Action(Act::Fail));
	state8.actions.push_back(Action(Act::Fail));
	state8.actions.push_back(Action(Act::Shift, 11));
	state8.actions.push_back(Action(Act::Fail));
	state8.gotos.push_back(Goto());
	state8.gotos.push_back(Goto());
	state8.gotos.push_back(Goto());
	LRTable.table.push_back(state8);

	State state9;
	state9.actions.push_back(Action(Act::Fail));
	state9.actions.push_back(Action(Act::Reduce, 1));
	state9.actions.push_back(Action(Act::Shift, 7));
	state9.actions.push_back(Action(Act::Fail));
	state9.actions.push_back(Action(Act::Reduce, 1));
	state9.actions.push_back(Action(Act::Reduce, 1));
	state9.gotos.push_back(Goto());
	state9.gotos.push_back(Goto());
	state9.gotos.push_back(Goto());
	LRTable.table.push_back(state9);

	State state10;
	state10.actions.push_back(Action(Act::Fail));
	state10.actions.push_back(Action(Act::Reduce, 3));
	state10.actions.push_back(Action(Act::Reduce, 3));
	state10.actions.push_back(Action(Act::Fail));
	state10.actions.push_back(Action(Act::Reduce, 3));
	state10.actions.push_back(Action(Act::Reduce, 3));
	state10.gotos.push_back(Goto());
	state10.gotos.push_back(Goto());
	state10.gotos.push_back(Goto());
	LRTable.table.push_back(state10);

	State state11;
	state11.actions.push_back(Action(Act::Fail));
	state11.actions.push_back(Action(Act::Reduce, 5));
	state11.actions.push_back(Action(Act::Reduce, 5));
	state11.actions.push_back(Action(Act::Fail));
	state11.actions.push_back(Action(Act::Reduce, 5));
	state11.actions.push_back(Action(Act::Reduce, 5));
	state11.gotos.push_back(Goto());
	state11.gotos.push_back(Goto());
	state11.gotos.push_back(Goto());
	LRTable.table.push_back(state11);
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
	parser.attGrammers.push_back(attGrammer2);

	// T->T*F
	AttGrammer attGrammer3;
	attGrammer3.left = "T";
	attGrammer3.right.push_back("T");
	attGrammer3.right.push_back("*");
	attGrammer3.right.push_back("F");
	parser.attGrammers.push_back(attGrammer3);

	// E->T
	AttGrammer attGrammer4;
	attGrammer4.left = "T";
	attGrammer4.right.push_back("F");
	parser.attGrammers.push_back(attGrammer4);

	// T->T*F
	AttGrammer attGrammer5;
	attGrammer5.left = "F";
	attGrammer5.right.push_back("(");
	attGrammer5.right.push_back("E");
	attGrammer5.right.push_back(")");
	parser.attGrammers.push_back(attGrammer5);

	// F->i
	AttGrammer attGrammer6;
	attGrammer6.left = "F";
	attGrammer6.right.push_back("Number");
	parser.attGrammers.push_back(attGrammer6);

	vector<string> str;
	str.push_back("123");
	str.push_back("*");
	str.push_back("-345");
	str.push_back("+");
	str.push_back("457.22");
	str.push_back("#");

	parser.init(); // 初始化一下
	std::cout << parser.parse(str) << std::endl;
	return 0;
}