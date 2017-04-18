#include "translator.h"

translator::translator() {
    registers_map["%eax"] = "$t0";
    registers_map["%ecx"] = "$t1";
    registers_map["%edx"] = "$t2";
    registers_map["%ebx"] = "$s0";
    registers_map["%esi"] = "$s1";
    registers_map["%edi"] = "$s2";
}

string translator::translate_IA32_to_MIPS(parser parser) {
    string output = "";
    for (vector<block*>::iterator b_iter = parser.get_code_blocks().begin(); 
        b_iter != parser.get_code_blocks().end(); b_iter++) {
        if ((*b_iter) -> get_label() != "") { // add procedure head
            output += ".globl " + (*b_iter) -> get_label() + "\n";
            output += ".ent " + (*b_iter) -> get_label() + "\n";
            output += (*b_iter) -> get_label() + ":\n";
        }
        for (vector<instruction*>::iterator i_iter = (*b_iter) -> get_instructions().begin(); 
            i_iter != (*b_iter) -> get_instructions().end(); i_iter++) {
            string translated_insts = "";
            if ((*i_iter) -> get_op() == "movl") {
                translated_insts += translate_movl((*i_iter));
            } else if ((*i_iter) -> get_op() == "addl") {
                translated_insts += translate_addl((*i_iter));
            } else if ((*i_iter) -> get_op() == "subl") {
                translated_insts += translate_subl((*i_iter));
            } else if ((*i_iter) -> get_op() == "imull") {

            } else if ((*i_iter) -> get_op() == "sall" || (*i_iter) -> get_op() == "shll" ) {

            } else if ((*i_iter) -> get_op() == "sarl") {

            } else if ((*i_iter) -> get_op() == "shrl") {

            } else if ((*i_iter) -> get_op() == "xorl") {

            } else if ((*i_iter) -> get_op() == "andl") {

            } else if ((*i_iter) -> get_op() == "orl") {

            } else if ((*i_iter) -> get_op() == "incl") {

            } else if ((*i_iter) -> get_op() == "decl") {

            } else if ((*i_iter) -> get_op() == "negl") {

            } else if ((*i_iter) -> get_op() == "notl") {

            } 
            output += translated_insts + "\n";
        }
        if ((*b_iter) -> get_label() != "") { // add procedure end
            output += ".end " + (*b_iter) -> get_label() + "\n";
        }
        output += "\n";
    }
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
        return "Wrong input instruction.";
    }
    return translated_inst;
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
        return "Wrong input instruction.";
    }
    return translated_inst;
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
        return "Wrong input instruction.";
    }
    return translated_inst;
}

translator::~translator() {
    registers_map.clear();
}