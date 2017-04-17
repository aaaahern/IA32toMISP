#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "instruction.h"

using namespace std;

class parser {
private:
	string file_name;

	vector<block*> code_blocks;
	unordered_map<string, int> label_dic;

	/** helper method **/
	string filter_comment(string line);
	bool is_label(string line);
	string get_label(string line);
	instruction extract_instruction(string line);

public:
	parser(string file_name);

	vector<block*> get_code_blocks();
	unordered_map<string, int> get_label_dic();

};

#undef PARSER_H
