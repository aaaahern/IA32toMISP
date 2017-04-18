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

