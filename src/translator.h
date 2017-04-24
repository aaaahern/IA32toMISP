#ifndef TRANSLATOR_H
#define TRANSLATOR_H
 
#include <unordered_map>
#include <string>
#include <vector>
#include <ctype.h>
#include "parser.h"
#include <initializer_list>


using namespace std;

class translator {
private:
    unordered_map<string, string> registers_map;

	const string WRONG_INSTRUCTION_MESG = "Wrong input instruction\n";

    /** instruction translation functions **/
    string translate_movl(instruction* inst);
    string translate_addl_andl_xorl_orl(instruction* inst);
    string translate_subl(instruction* inst);
    string translate_imull(instruction* inst);
    string translate_idivl(instruction* inst);
    string translate_sall_or_shll(instruction* inst);
    string translate_sarl(instruction* inst);
    string translate_shrl(instruction* inst);
    string translate_incl(instruction* inst);
    string translate_decl(instruction* inst);
    string translate_negl(instruction* inst);
    string translate_notl(instruction* inst);
	string translate_pushl(instruction* inst);
	string translate_batch_pushl(vector<instruction*> instructions);
	string translate_popl(instruction* inst);
	string translate_call(instruction* inst);
	string translate_call_with_arguments(vector<instruction*> instructions, int argument_count);
	string translate_jmp(instruction* inst);
	string translate_cmpl_j(instruction* cmpl_inst, instruction* j_inst);
    string translate_prn(instruction* inst);

	string translate_procedure_head();
	string translate_procedure_end();


	/** addressing helper functions **/
	bool is_immediate(string operand);
	bool is_register(string operand);
	bool is_indirect(string operand);
	bool is_absolute(string operand);
	bool is_indexed(string operand);
	bool is_scaled_indexed(string operand);

	string map_indirect(string operand);
	string map_immediate(string operand);

	string address_absolute(string& translated_insts, string operand);
	string address_indexed(string& translated_insts, string operand);
	string address_scaled_indexed(string& translated_insts, string operand);

public:
    translator();
    string translate_IA32_to_MIPS(parser parser);
    ~translator();
};
#endif
