#ifndef BLOCK_H
#define BLOCK_H

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
}

#undef BLOCK_H
