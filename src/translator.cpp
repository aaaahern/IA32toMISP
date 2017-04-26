#include "translator.h"
#include <iostream>
#include <algorithm>
#include <sstream>

translator::translator() {
    registers_map["%eax"] = "$t0";
    registers_map["%ecx"] = "$t1";
    registers_map["%edx"] = "$t2";
    registers_map["%ebx"] = "$s0";
    registers_map["%esi"] = "$s1";
    registers_map["%edi"] = "$s2";
    registers_map["%esp"] = "$sp";
    registers_map["%ebp"] = "$fp";
	registers_map["temp"] = "$s7";
	registers_map["addressing_result"] = "$s6";
	registers_map["zero"] = "$zero";
}

string translator::translate_IA32_to_MIPS(parser parser) {
    string output = "";
    // TODO translate .data
    output += ".data\n\tnewline: .asciiz \"\\n\"\n";
    output += ".text\n";
	vector<block*> blocks = parser.get_code_blocks();
    bool is_procedure_head = true;
    string procedure_name;
    for (auto b_iter = blocks.begin(); b_iter != blocks.end(); b_iter++) {
		block* block = *b_iter;
        if (block->get_label() != "") { // add procedure head label
            if (is_procedure_head) {
                output += ".globl " + block->get_label() + "\n";
                output += ".ent " + block->get_label() + "\n";
                procedure_name = block->get_label();
                is_procedure_head = false;
            }
            output += block->get_label() + ":\n";
        }

		vector<instruction*> instructions = block->get_instructions();
        for (auto i_iter = instructions.begin(); i_iter != instructions.end(); ) {
            string translated_insts = "";
			instruction* instr = *i_iter;
			string op = instr->get_op();

            if (op == "movl") {
                translated_insts += translate_movl(instr);
				i_iter++;
            } else if (op == "addl" || op == "xorl" || op == "andl" || op == "orl") {
                translated_insts += translate_addl_andl_xorl_orl(instr);
				i_iter++;
            } else if (op == "subl") {
                translated_insts += translate_subl(instr);
				i_iter++;
            } else if (op == "imull") {
                translated_insts += translate_imull(instr);
				i_iter++;
            } else if (op == "idivl") {
                translated_insts += translate_idivl(instr);
                i_iter++;
            } else if (op == "sall" || op == "shll" ) {
                translated_insts += translate_sall_or_shll(instr);
				i_iter++;
            } else if (op == "sarl") {
                translated_insts += translate_sarl(instr);
				i_iter++;
            } else if (op == "shrl") {
                translated_insts += translate_shrl(instr);
				i_iter++;
            } else if (op == "incl") {
                translated_insts += translate_incl(instr);
				i_iter++;
            } else if (op == "decl") {
                translated_insts += translate_decl(instr);
				i_iter++;
            } else if (op == "negl") {
                translated_insts += translate_negl(instr);
				i_iter++;
            } else if (op == "notl") {
                translated_insts += translate_notl(instr);
				i_iter++;
            } else if (op == "pushl") {
				if (instr->get_operand1() == "%ebp") {
					// procedure head setup
					translated_insts += translate_procedure_head();
					i_iter++;
					i_iter++;
				} else {
					vector<instruction*> inst_buffer;
					int argument_count = 0;

					while (i_iter != instructions.end() && (*i_iter)->get_op() == "pushl") {
						inst_buffer.push_back(*i_iter);
						i_iter++;
						argument_count++;
					}

					if (i_iter != instructions.end() && (*i_iter)->get_op() == "call") {
						// procedure arguments
						inst_buffer.push_back(*i_iter);
						i_iter++;
						translated_insts += translate_call_with_arguments(inst_buffer, argument_count);
					} else {
						// normal pushl
						translated_insts += translate_batch_pushl(inst_buffer);
					}
				}
			} else if (op == "popl") {
				translated_insts += translate_popl(instr);
				i_iter++;
            } else if (op == "leave") {
				// procedure end setup
				translated_insts += translate_procedure_end();
                is_procedure_head = true;
				i_iter++;
				i_iter++;
			} else if (op == "call") {
				translated_insts += translate_call(instr);
				i_iter++;
			} else if (op == "cmpl") {
				instruction* cmpl_inst = instr;
				i_iter++;
				instruction* j_inst = *i_iter;
				i_iter++;
				translated_insts += translate_cmpl_j(cmpl_inst, j_inst);
			} else if (op == "jmp") {
                translated_insts += translate_jmp(instr);
                i_iter++;
            } else if (op == "prn") {
                translated_insts += translate_prn(instr);
                i_iter++;
            } else if (op == "cltd") {
                i_iter++;
            }

            output += translated_insts;
        }

        if (block->get_label() != "" && is_procedure_head) { // add procedure end label
            output += ".end " + procedure_name + "\n";
        }
        output += "\n";
    }
    return output;
}

string translator::translate_prn(instruction* inst) {
    string translated_inst = "";
    translated_inst += instruction::to_string(1, "add", {"$a0", "$zero", registers_map[inst->get_operand1()]});
    translated_inst += instruction::to_string(1, "li", {"$v0", "1"});
    translated_inst += instruction::to_string(1, "syscall", {});
    translated_inst += instruction::to_string(1, "li", {"$v0", "4"});
    translated_inst += instruction::to_string(1, "la", {"$a0", "newline"});
    translated_inst += instruction::to_string(1, "syscall", {});
    return translated_inst;
}

string translator::translate_procedure_head() {
	string translated_inst = "";
	translated_inst += instruction::to_string(1, "addi", {"$sp", "$sp", "-8"});
	translated_inst += instruction::to_string(1, "sw", {"$ra", "4($sp)"});
	translated_inst += instruction::to_string(1, "sw", {"$fp", "0($sp)"});
	translated_inst += instruction::to_string(1, "addi", {"$fp", "$sp", "0"});

	return translated_inst;
}

string translator::translate_procedure_end() {
	string translated_inst = "";
	translated_inst += instruction::to_string(1, "lw", {"$fp", "0($sp)"});
	translated_inst += instruction::to_string(1, "lw", {"$ra", "4($sp)"});
	translated_inst += instruction::to_string(1, "add", {"$sp", "$sp", "8"});
	translated_inst += instruction::to_string(1, "jr", {"$ra"});

	return translated_inst;
}

string translator::translate_call(instruction* inst) {
	return instruction::to_string(1, "jal", {inst->get_operand1()});
}

string translator::translate_call_with_arguments(vector<instruction*> instructions, int argument_count) {
	string translated_inst = "";

	int i = 0;
	for (; i < argument_count; i++) {
		translated_inst += translate_pushl(instructions[i]);
	}

	// last instruction is "call"
	translated_inst += translate_call(instructions[i]);

	translated_inst += instruction::to_string(1, "addi", {"$sp", "$sp", to_string(4 * argument_count)});

	return translated_inst;
}

string translator::translate_pushl(instruction* inst) {
	string operand = inst->get_operand1();
	string immediate = operand.substr(1, operand.length()-1);

	string translated_inst = instruction::to_string(1, "addi", {"$sp", "$sp", "-4"});

	if (is_immediate(operand)) {
		translated_inst += instruction::to_string(1, "li", {registers_map["temp"], immediate});
		translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], "0($sp)"});
	} else if (is_register(operand)) {
		translated_inst += instruction::to_string(1, "sw", {registers_map[operand], "0($sp)"});
	} else {
		return WRONG_INSTRUCTION_MESG;
	}
	return translated_inst;
}

string translator::translate_batch_pushl(vector<instruction*> instructions) {
	string translated_inst = "";
	for (auto iter = instructions.begin(); iter != instructions.end(); iter++) {
		translated_inst += translate_pushl(*iter);
	}
	return translated_inst;
}

string translator::translate_popl(instruction* inst) {
	string translated_inst = "";
	translated_inst += instruction::to_string(1, "lw", {registers_map[inst->get_operand1()], "0($sp)"});
	translated_inst += instruction::to_string(1, "addi", {"$sp", "$sp", "4"});
	return translated_inst;
}

string translator::translate_movl(instruction* inst) {
    bool is_wrong_inst = false;
    string translated_inst = "";
	string operand1 = inst->get_operand1();
	string operand2 = inst->get_operand2();

    if (operand1 == "" || operand2 == "") {
        is_wrong_inst = true;
    }

    if (is_register(operand1)) { // first operand is register
        if (is_register(operand2)) { // second operand is register
			translated_inst += instruction::to_string(1, "add", {registers_map[operand2], registers_map["zero"], registers_map[operand1]});
        } else if (is_indirect(operand2)) { // second operand is indirect
			translated_inst += instruction::to_string(1, "sw", {registers_map[operand1], map_indirect(operand2)});
		} else if (is_absolute(operand2)) {
			string new_operand2 = address_absolute(translated_inst, operand2);
			translated_inst += instruction::to_string(1, "sw", {registers_map[operand1], new_operand2});
		} else if (is_indexed(operand2)) {
			string new_operand2 = address_indexed(translated_inst, operand2);
			translated_inst += instruction::to_string(1, "sw", {registers_map[operand1], new_operand2});
		} else if (is_scaled_indexed(operand2)) {
			string new_operand2 = address_scaled_indexed(translated_inst, operand2);
			translated_inst += instruction::to_string(1, "sw", {registers_map[operand1], new_operand2});
        } else {
            is_wrong_inst = true;
        }
    } else if (is_immediate(operand1)) { // first operand is immediate
        string immediate = map_immediate(operand1);

        if (is_register(operand2)) { // second operand is register
			translated_inst += instruction::to_string(1, "li", {registers_map[operand2], immediate});
        } else if (is_indirect(operand2)) { // second operand is indirect
			translated_inst += instruction::to_string(1, "li", {registers_map["temp"], immediate});
			translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], map_indirect(operand2)});
        } else if (is_absolute(operand2)) { 
			string new_operand2 = address_absolute(translated_inst, operand2);
			translated_inst += instruction::to_string(1, "li", {registers_map["temp"], immediate});
			translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], new_operand2});
        } else if (is_indexed(operand2)) {
			string new_operand2 = address_indexed(translated_inst, operand2);
			translated_inst += instruction::to_string(1, "li", {registers_map["temp"], immediate});
			translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], new_operand2});
        } else if (is_scaled_indexed(operand2)) {
			string new_operand2 = address_scaled_indexed(translated_inst, operand2);
			translated_inst += instruction::to_string(1, "li", {registers_map["temp"], immediate});
			translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], new_operand2});
        } else {
            is_wrong_inst = true;
        }
    } else if (is_indirect(operand1)) { // first operand is address
        if (is_register(operand2)) { // second operand is register
			translated_inst += instruction::to_string(1, "lw", {registers_map[operand2], map_indirect(operand1)});
        } else {
            is_wrong_inst = true;
        }
    } else if (is_absolute(operand1)) { 
        if (is_register(operand2)) { // second operand is register
			string new_operand1 = address_absolute(translated_inst, operand1);
			translated_inst += instruction::to_string(1, "lw", {registers_map[operand2], new_operand1});
        } else {
            is_wrong_inst = true;
        }
    } else if (is_indexed(operand1)) { 
        if (is_register(operand2)) { // second operand is register
			string new_operand1 = address_indexed(translated_inst, operand1);
			translated_inst += instruction::to_string(1, "lw", {registers_map[operand2], new_operand1});
        } else {
            is_wrong_inst = true;
        }
    } else if (is_scaled_indexed(operand1)) { 
        if (is_register(operand2)) { // second operand is register
			string new_operand1 = address_scaled_indexed(translated_inst, operand1);
			translated_inst += instruction::to_string(1, "lw", {registers_map[operand2], new_operand1});
        } else {
            is_wrong_inst = true;
        }
    } else {
        is_wrong_inst = true;
    }
    if (is_wrong_inst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst;
}

string translator::translate_addl_andl_xorl_orl(instruction* inst) {
    string op = inst -> get_op();
    op = op.substr(0, op.length()-1);
    string operand1 = inst -> get_operand1();
    string operand2 = inst -> get_operand2();

    if (is_register(operand1) && is_register(operand2)) { 
		return instruction::to_string(1, op, {registers_map[operand2], registers_map[operand2], registers_map[operand1]});
    } else if (is_immediate(operand1)) {  
        string immediate = map_immediate(operand1);
        if (is_register(operand2)) { // second operand is register
			return instruction::to_string(1, op + "i", {registers_map[operand2], registers_map[operand2], immediate});
        } else if (is_indirect(operand2)) { // second operand is address
			string new_operand2 = map_indirect(operand2);
			string translated_inst;
			translated_inst += instruction::to_string(1, op, {registers_map["temp"], registers_map["zero"], immediate});
			translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], new_operand2});
			return translated_inst;
        } else {
			return WRONG_INSTRUCTION_MESG;
        }
    } else if (is_indirect(operand1) && is_register(operand2)) { 
		string new_operand1 = map_indirect(operand1);
		string translated_inst;
		translated_inst += instruction::to_string(1, "lw", {registers_map["temp"], new_operand1});
		translated_inst += instruction::to_string(1, op, {registers_map[operand2], registers_map[operand2], registers_map["temp"]});
		return translated_inst;
	} else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_subl(instruction* inst) {
    string translated_inst;
	string operand1 = inst->get_operand1();
	string operand2 = inst->get_operand2();
    if (is_register(operand1) && is_register(operand2)) { 
		return instruction::to_string(1, "sub", {registers_map[operand2], registers_map[operand2], registers_map[operand1]});
    } else if (is_immediate(operand1) && is_register(operand2)) {
        string immediate = map_immediate(operand1);
		if (immediate.at(0) == '-') {
			immediate = immediate.substr(1, immediate.length() - 1 );
		} else {
			immediate = "-" + immediate;
		}
		return instruction::to_string(1, "addi", {registers_map[operand2], registers_map[operand2], immediate});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_imull(instruction* inst) {
	string operand1 = inst->get_operand1();
	string operand2 = inst->get_operand2();
	string translated_inst = "";
    if (is_register(operand1) && is_register(operand2)) { 
		translated_inst += instruction::to_string(1, "mult", {registers_map[operand1], registers_map[operand2]});
		translated_inst += instruction::to_string(1, "mflo", {registers_map[operand2]});
		return translated_inst;
    } else if (is_immediate(operand1) && is_register(operand2)) { 
		string immediate = map_immediate(operand1);
		string temp_register = "$s6";
		translated_inst += instruction::to_string(1, "addi", {temp_register, registers_map["zero"], immediate});
		translated_inst += instruction::to_string(1, "mult", {temp_register, registers_map[operand2]});
		translated_inst += instruction::to_string(1, "mflo", {registers_map[operand2]});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_idivl(instruction* inst) {
    string operand = inst->get_operand1();
    string translated_inst = "";
    if (is_register(operand)) {
        translated_inst += instruction::to_string(1, "div", {registers_map["%eax"], registers_map[operand]});
        translated_inst += instruction::to_string(1, "mflo", {registers_map["%eax"]});
        translated_inst += instruction::to_string(1, "mfhi", {registers_map["%edx"]});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst;
}

string translator::translate_sall_or_shll(instruction* inst) {
	string operand1 = inst->get_operand1();
	string operand2 = inst->get_operand2();
    if (is_register(operand1) && is_register(operand2)) { 
		return instruction::to_string(1, "sllv", {registers_map[operand2], registers_map[operand2], registers_map[operand1]});
    } else if (is_immediate(operand1) && is_register(operand2)) {  
		return instruction::to_string(1, "sll", {registers_map[operand2], registers_map[operand2], map_immediate(operand1)});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_sarl(instruction* inst) {
	string operand1 = inst->get_operand1();
	string operand2 = inst->get_operand2();
    if (is_immediate(operand1) && is_register(operand2)) { 
		string immediate = map_immediate(operand1);
		return instruction::to_string(1, "sra", {registers_map[operand2], registers_map[operand2], immediate});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_shrl(instruction* inst) {
	string operand1 = inst->get_operand1();
	string operand2 = inst->get_operand2();
    if (is_immediate(operand1) && is_register(operand2)) {
		string immediate = map_immediate(operand1);
		return instruction::to_string(1, "srl", {registers_map[operand2], registers_map[operand2], immediate});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_incl(instruction* inst) {
	string operand = inst->get_operand1();
    if (is_register(operand)) { // first operand is register
		return instruction::to_string(1, "addi", {registers_map[operand], registers_map[operand], "1"});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_decl(instruction* inst) {
	string operand = inst->get_operand1();
    if (is_register(operand)) { // first operand is register
		return instruction::to_string(1, "addi", {registers_map[operand], registers_map[operand], "-1"});
    } else if (is_indirect(operand)) {
        string new_operand = map_indirect(operand);
        string translated_inst;
        translated_inst += instruction::to_string(1, "lw", {registers_map["temp"], new_operand});
        translated_inst += instruction::to_string(1, "addi", {registers_map["temp"], registers_map["temp"], "-1"});
        translated_inst += instruction::to_string(1, "sw", {registers_map["temp"], new_operand});
        return translated_inst;
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_negl(instruction* inst) {
	string operand = inst->get_operand1();
    if (is_register(operand)) { // first operand is register
		return instruction::to_string(1, "sub", {registers_map[operand], registers_map["zero"], registers_map[operand]});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_notl(instruction* inst) {
	string operand = inst->get_operand1();
    if (is_register(operand)) { // first operand is register
		return instruction::to_string(1, "not", {registers_map[operand], registers_map[operand]});
    } else {
        return WRONG_INSTRUCTION_MESG;
    }
}

string translator::translate_jmp(instruction* inst) {
	return instruction::to_string(1, "b", {inst->get_operand1()});
}

string translator::translate_cmpl_j(instruction* cmpl_inst, instruction* j_inst) {
	string Rsrc1 = cmpl_inst->get_operand2();
    string src2 = cmpl_inst->get_operand1();
    if (is_register(Rsrc1)) { // register
        Rsrc1 = registers_map[Rsrc1];
    } else if (is_immediate(Rsrc1)) { // immediate
        Rsrc1 = map_immediate(Rsrc1);
    }
	if (is_register(src2)) { // register
        src2 = registers_map[src2];
    } else if (is_immediate(src2)) { // immediate
		src2 = map_immediate(src2);
    }
    string j_label = j_inst->get_operand1();
	string j_op = j_inst->get_op();
	string translated_inst = "";

	if (j_op == "je") {
		translated_inst += instruction::to_string(1, "beq", {Rsrc1, src2, j_label});
	} else if (j_op == "jne") {
		translated_inst += instruction::to_string(1, "bne", {Rsrc1, src2, j_label});
	} else if (j_op == "jl") {
		translated_inst += instruction::to_string(1, "blt", {Rsrc1, src2, j_label});
	} else if (j_op == "jle") {
		translated_inst += instruction::to_string(1, "ble", {Rsrc1, src2, j_label});
	} else if (j_op == "jg") {
		translated_inst += instruction::to_string(1, "bgt", {Rsrc1, src2, j_label});
	} else if (j_op == "jge") {
		translated_inst += instruction::to_string(1, "bge", {Rsrc1, src2, j_label});
	}

	return translated_inst += "\n";
}

bool translator::is_immediate(string operand) {
	return operand.size() > 0 && operand.at(0) == '$';
}

bool translator::is_register(string operand) {
	return operand.size() > 0 && operand.at(0) == '%';
}

bool translator::is_absolute(string operand) {
	return all_of(operand.begin(), operand.end(), ::isdigit); 
}

bool translator::is_indirect(string operand) {
	return operand.find('(') != string::npos
		&& operand.find(')') != string::npos
		&& operand.find(',') == string::npos;
}

bool translator::is_indexed(string operand) {
	int comma_count = 0;
	for (char c : operand) {
		if (c == ',') {
			comma_count++;
		}
	}
	return comma_count == 1
		&& operand.find('(') != string::npos
		&& operand.find(')') != string::npos;
}

bool translator::is_scaled_indexed(string operand) {
	int comma_count = 0;
	for (char c : operand) {
		if (c == ',') {
			comma_count++;
		}
	}
	return comma_count == 2
		&& operand.find('(') != string::npos
		&& operand.find(')') != string::npos;
}


string translator::address_absolute(string& translated_insts, string operand) {
	string result_register = registers_map["addressing_result"];
	translated_insts += instruction::to_string(1, "addi", {result_register, registers_map["zero"], operand});
	return "(" + result_register + ")";
}

string translator::address_indexed(string& translated_insts, string operand) {
	operand.erase( remove_if( operand.begin(), operand.end(), ::isspace ), operand.end() );

	size_t i = operand.find('(');
	size_t j = operand.find(')');

	string imm = operand.substr(0, i);
	string paren_content = operand.substr(i+1, j-i-1);

	string reg1, reg2;
	istringstream ss(paren_content);
	getline(ss, reg1, ',');
	getline(ss, reg2, ',');
	if (imm.empty()) {
		imm = "0";
	}
	string result_register = registers_map["addressing_result"];
	string new_operand = imm + "(" + result_register + ")";

	translated_insts += instruction::to_string(1, "add", {result_register, registers_map[reg1], registers_map[reg2]});

	return new_operand;
}

string translator::address_scaled_indexed(string& translated_insts, string operand) {
	operand.erase( remove_if( operand.begin(), operand.end(), ::isspace ), operand.end() );

	size_t i = operand.find('(');
	size_t j = operand.find(')');

	string imm1 = operand.substr(0, i);
	string paren_content = operand.substr(i+1, j-i-1);

	string reg1, reg2, imm2;
	istringstream ss(paren_content);
	getline(ss, reg1, ',');
	getline(ss, reg2, ',');
	getline(ss, imm2, ',');

	if (imm1.empty()) {
		imm1 = "0";
	}
	if (reg1.empty()) {
		reg1 = registers_map["zero"];
	} else {
		reg1 = registers_map[reg1];
	}
	string result_register = registers_map["addressing_result"];
	string new_operand = imm1 + "(" + result_register + ")";

	translated_insts += instruction::to_string(1, "addi", {result_register, registers_map["zero"], imm2});
	translated_insts += instruction::to_string(1, "mult", {result_register, registers_map[reg2]});
	translated_insts += instruction::to_string(1, "mflo", {result_register});
	translated_insts += instruction::to_string(1, "add", {result_register, result_register, reg1});

	return new_operand;
}

string translator::map_indirect(string operand) {
	size_t i = operand.find("(");
	size_t j = operand.find(")");
	string offset = operand.substr(0, i);
	string dst_register = operand.substr(i+1, j-i-1);
	string new_operand = offset + "(" + registers_map[dst_register] + ")";

	return new_operand;
}

string translator::map_immediate(string operand) {
    return operand.substr(1, operand.size() - 1);
}

translator::~translator() {
    registers_map.clear();
}
