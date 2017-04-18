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

public:
    translator();
    string translate_IA32_to_MIPS(parser parser);
    ~translator();
};
#endif