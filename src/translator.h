#ifndef TRANSLATOR_H
#define TRANSLATOR_H
 
#include <unordered_map>
#include <string>
#include <vector>
#include <ctype.h>
#include "parser.h"

using namespace std;

class translator {
private:
    unordered_map<string, string> registers_map;

    /** helper functions **/
    string translate_movl(instruction*);
    string translate_addl(instruction*);
    string translate_subl(instruction*);
    string translate_imull(instruction*);
    string translate_sall_or_shll(instruction*);
    string translate_sarl(instruction*);
    string translate_shrl(instruction*);
    string translate_xorl(instruction*);
    string translate_andl(instruction*);
    string translate_orl(instruction*);
    string translate_incl(instruction*);
    string translate_decl(instruction*);
    string translate_negl(instruction*);
    string translate_notl(instruction*);

public:
    translator();
    string translate_IA32_to_MIPS(parser parser);
    ~translator();
};
#endif