#include "instruction.h"


instruction::instruction(string op, string operand1, string operand2) :
	op(op), operand1(operand1), operand2(operand2) {}

instruction::instruction(string op, string operand1) :
	op(op), operand1(operand1), operand2("") {}

instruction::instruction(string op) : 
	op(op), operand1(""), operand2("") {}
	
string instruction::get_op() {
	return op;
}

string instruction::get_operand1() {
	return operand1;
}

string instruction::get_operand2() {
	return operand2;
}

string instruction::to_string(int tab_num, string op, initializer_list<string> operands) {
	string instr_string = "";
			
	instr_string.append(tab_num, '\t');
	instr_string.append(op);
					
	if (operands.size() > 0) {
		instr_string.append(" ");
		auto iter = operands.begin();
		instr_string.append(*iter);
		iter++;
												
		for (; iter != operands.end(); iter++) {
			instr_string.append(", ");
			instr_string.append(*iter);
		}
	}

	return instr_string + "\n";
}
