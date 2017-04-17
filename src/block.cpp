#include "block.h"

block::block(string label) : label(label) {}

string block::get_label() {
	return label;
}

vector<instruction*> block::get_instructions() {
	return instructions;
}

void block::push_back_instruction(instruction* instr) {
	instructions.push_back(instr);
}

block::~block() {
	for (auto instr = instructions.begin(); instr != instructions.end(); instr++) {
		delete *instr;
	}
}
