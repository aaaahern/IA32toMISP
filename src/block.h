#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "instruction.h"

using namespace std;

class block {
private:
	string label;
	vector<instruction*> instructions;

public:
	block(string label);

	string get_label();
	vector<instruction*> get_instructions();
	void push_back_instruction(instruction* instr);

	~block();
};

#endif 
