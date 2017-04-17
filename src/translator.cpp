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
    for (vector<block*>::iterator b_iter = parser.get_code_blocks().begin(); 
        b_iter != parser.get_code_blocks().end(); b_iter++) {
        for (vector<instruction*>::iterator i_iter = (*b_iter) -> get_instructions().begin(); 
            i_iter != (*b_iter) -> get_instructions().end(); i_iter++) {
            if ((*i_iter) -> get_op() == "movl") {

            } else if ((*i_iter) -> get_op() == "addl") {

            } else if ((*i_iter) -> get_op() == "subl") {

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
        }
    }
}

translator::~translator() {
    registers_map.clear();
}