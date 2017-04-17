#ifndef TRANSLATOR_H
#define TRANSLATOR_H
 
#include <unordered_map>
#include <string>
#include <vector>
#include "parser.h"

using namespace std;

class translator {
private:
    string output_path;

    unordered_map<string, string> registers_map;

public:
    translator();
    string translate_IA32_to_MIPS(parser parser);
    ~translator();
};
#endif