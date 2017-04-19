#include "translator.h"
#include <iostream>

translator::translator() {
    registers_map["%eax"] = "$t0";
    registers_map["%ecx"] = "$t1";
    registers_map["%edx"] = "$t2";
    registers_map["%ebx"] = "$s0";
    registers_map["%esi"] = "$s1";
    registers_map["%edi"] = "$s2";
	registers_map["temp"] = "$s7";
}

string translator::translate_IA32_to_MIPS(parser parser) {
    string output = "";
	vector<block*> blocks = parser.get_code_blocks();
    for (auto b_iter = blocks.begin(); b_iter != blocks.end(); b_iter++) {
		block* block = *b_iter;
        if (block->get_label() != "") { // add procedure head label
            output += ".globl " + block->get_label() + "\n";
            output += ".ent " + block->get_label() + "\n";
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
            } else if (op == "addl") {
                translated_insts += translate_addl(instr);
				i_iter++;
            } else if (op == "subl") {
                translated_insts += translate_subl(instr);
				i_iter++;
            } else if (op == "imull") {
                translated_insts += translate_imull(instr);
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
            } else if (op == "xorl") {
                translated_insts += translate_xorl(instr);
				i_iter++;
            } else if (op == "andl") {
                translated_insts += translate_andl(instr);
				i_iter++;
            } else if (op == "orl") {
                translated_insts += translate_orl(instr);
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
				instr++;
            } else if (op == "leave") {
				// procedure end setup
				translated_insts += translate_procedure_end();
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
			}

            output += translated_insts;
        }

        if (block->get_label() != "") { // add procedure end label
            output += ".end " + block->get_label() + "\n";
        }
        output += "\n";
    }

	cout << output << endl;
}

string translator::translate_procedure_head() {
	return "addi $sp, $sp, -4\nsw $ra, 0($sp)\n";
}

string translator::translate_procedure_end() {
	return "lw $ra, 0($sp)\naddi $sp, $sp, 4\njr $ra\n";
}

string translator::translate_call(instruction* inst) {
	return "jal " + inst->get_operand1() + "\n";
}

string translator::translate_call_with_arguments(vector<instruction*> instructions, int argument_count) {
	string translated_inst = "";

	int i = 0;
	for (; i < argument_count; i++) {
		translated_inst += translate_pushl(instructions[i]);
	}

	// last instruction is "call"
	translated_inst += translate_call(instructions[i]);

	for (; i > 0; i--) {
		translated_inst += "addi $sp, $sp, 4\n";
	}

	return translated_inst;
}

string translator::translate_pushl(instruction* inst) {
	string operand = inst->get_operand1();
	string immediate = operand.substr(1, operand.length()-1);
    
	string translated_inst = "addi $sp, $sp, -4\n";

	if (is_immediate(operand)) {
		translated_inst += "li " + registers_map["temp"] + ", " + immediate + "\n";
		translated_inst += "sw " + registers_map["temp"] + ", 0($sp)\n";
	} else if (is_register(operand)) {
		translated_inst += "sw " + operand + ", 0($sp)\n";
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
	return "lw " + inst->get_operand1() + ", 0($sp)\n" 
		+ "addi $sp, $sp, 4\n";
}

string translator::translate_movl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;

    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "add " + registers_map[inst -> get_operand2()] + 
                ", $zero" + registers_map[inst -> get_operand1()];
        } else if (isdigit(inst -> get_operand2().at(0))) { // second operand is address
            int i = inst -> get_operand2().find("(");
            int j = inst -> get_operand2().find(")");
            string offset = inst -> get_operand2().substr(0, i);
            string dst_register = inst -> get_operand2().substr(i+1, j-i-1);
            translated_inst = "sw " + registers_map[inst -> get_operand1()] + ", " + 
                offset + "(" + registers_map[dst_register] + ")";
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') { // first operand is immediate
        string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "li " + registers_map[inst -> get_operand2()] + ", " + immediate;
        } else if (isdigit(inst -> get_operand2().at(0))) { // second operand is address
            int i = inst -> get_operand2().find("(");
            int j = inst -> get_operand2().find(")");
            string offset = inst -> get_operand2().substr(0, i);
            string dst_register = inst -> get_operand2().substr(i+1, j-i-1);
            string temp_register = "$s7"; // use $s7 as a temp register
            translated_inst = "li" + temp_register + "," + immediate + "\n" + 
                "sw " + temp_register + "," + offset + "(" + dst_register + ")"; 
        } else {
            isWrongInst = true;
        }
    } else if (isdigit(inst -> get_operand1().at(0))) { // first operand is address
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            int i = inst -> get_operand1().find("(");
            int j = inst -> get_operand1().find(")");
            string offset = inst -> get_operand1().substr(0, i);
            string src_register = inst -> get_operand1().substr(i+1, j-i-1);
            translated_inst = "lw " + registers_map[inst -> get_operand2()] + ", " + 
                offset + "(" + src_register + ")";
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_addl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "add " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + registers_map[inst -> get_operand1()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "addi " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_subl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "sub " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + registers_map[inst -> get_operand1()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "addi " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_imull(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "mult " + registers_map[inst -> get_operand1()] + ", " + 
                registers_map[inst -> get_operand2()] + "\n" + 
                "mflo " + registers_map[inst -> get_operand2()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            string temp_register = "$s6";
            translated_inst = "mult " + temp_register + ", " + "$zero" + ", " + immediate + "\n" +
                "mult " + temp_register + ", " + registers_map[inst -> get_operand2()] + "\n" + 
                "mflo " + registers_map[inst -> get_operand2()];
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_sall_or_shll(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "sllv " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + registers_map[inst -> get_operand1()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "sll " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_sarl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "sra " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_shrl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "srl " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_xorl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "xor " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + registers_map[inst -> get_operand1()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "xori " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_andl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "and " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + registers_map[inst -> get_operand1()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "andi " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_orl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "" || inst -> get_operand2() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            translated_inst = "or " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + registers_map[inst -> get_operand1()];
        } else {
            isWrongInst = true;
        }
    } else if (inst -> get_operand1().at(0) == '$') {  // first operand is immediate
        if (inst -> get_operand2().at(0) == '%') { // second operand is register
            string immediate = inst -> get_operand1().substr(1, inst -> get_operand1().length()-1);
            translated_inst = "ori " + registers_map[inst -> get_operand2()] + ", " + 
                registers_map[inst -> get_operand2()] + ", " + "-" + immediate;
        } else {
            isWrongInst = true;
        }
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_incl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        translated_inst = "addi " + registers_map[inst -> get_operand1()] + ", " + 
            registers_map[inst -> get_operand1()] + ", " + "1";
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_decl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        translated_inst = "addi " + registers_map[inst -> get_operand1()] + ", " + 
            registers_map[inst -> get_operand1()] + ", " + "-1";
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_negl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        translated_inst = "sub " + registers_map[inst -> get_operand1()] + ", " + 
            "$zero" + ", " + registers_map[inst -> get_operand1()];
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_notl(instruction* inst) {
    bool isWrongInst = false;
    string translated_inst;
    if (inst -> get_operand1() == "") {
        isWrongInst = true;
    }
    if (inst -> get_operand1().at(0) == '%') { // first operand is register
        translated_inst = "not " + registers_map[inst -> get_operand1()] + ", " + 
            registers_map[inst -> get_operand1()];
    } else {
        isWrongInst = true;
    }
    if (isWrongInst) {
        return WRONG_INSTRUCTION_MESG;
    }
    return translated_inst + "\n";
}

string translator::translate_jmp(instruction* inst) {
	return "b " + inst->get_operand1() + "\n";
}

string translator::translate_cmpl_j(instruction* cmpl_inst, instruction* j_inst) {
	string Rsrc1 = cmpl_inst->get_operand2();
	string src2 = cmpl_inst->get_operand1();
	string j_label = j_inst->get_operand1();
	string j_op = j_inst->get_op();
	string translated_inst = "";

	if (j_op == "je") {
		translated_inst += "beq " + Rsrc1 + ", " + src2 + ", " + j_label;
	} else if (j_op == "jne") {
		translated_inst += "bne " + Rsrc1 + ", " + src2 + ", " + j_label;
	} else if (j_op == "jl") {
		translated_inst += "bgt " + Rsrc1 + ", " + src2 + ", " + j_label;
	} else if (j_op == "jle") {
		translated_inst += "bge " + Rsrc1 + ", " + src2 + ", " + j_label;
	} else if (j_op == "jg") {
		translated_inst += "blt " + Rsrc1 + ", " + src2 + ", " + j_label;
	} else if (j_op == "jge") {
		translated_inst += "ble " + Rsrc1 + ", " + src2 + ", " + j_label;
	}

	return translated_inst += "\n";
}

bool translator::is_immediate(string operand) {
	return operand.size() > 0 && operand.at(0) == '$';
}

bool translator::is_register(string operand) {
	return operand.size() > 0 && operand.at(0) == '%';
}

translator::~translator() {
    registers_map.clear();
}
