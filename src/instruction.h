#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

using namespace std;

class instruction {
private:
	string op;
	string operand1;
	string operand2;

public:
	instruction(string op, string operand1, string operand2);
	instruction(string op, string operand1);
	instruction(string op);

	string get_op();
	string get_operand1();
	string get_operand2();

	static string to_string(int tab_num, string op, initializer_list<string> operands);
};


#endif 
