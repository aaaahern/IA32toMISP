#include "parser.h"
#include <fstream>
#include <sstream>

parser::parser(string file_name) {
	ifstream infile(file_name);
	string buffer;
	block* current_block = NULL;

	while (getline(infile, buffer)) {
		buffer = filter_comment(buffer);

		// new block, and update current_block pointer
		if (is_label(buffer)) {
			string label = get_label(buffer);
			block* new_block = new block(label);
			code_blocks.push_back(new_block);
			label_dic.insert({label, code_blocks.size() - 1});

			current_block = new_block;
		}

		instruction* new_instr = extract_instruction(buffer);
		if (new_instr != NULL) {
			current_block->push_back_instruction(new_instr);
		}
	}
}

string parser::filter_comment(string line) {
	size_t pound_pos = line.find("#");
	return line.substr(0, pound_pos);
}

bool parser::is_label(string line) {
	return line.find(":") != string::npos;
}

string parser::get_label(string line) {
	size_t column_pos = line.find(":");
	if (column_pos == string::npos) {
		return NULL;
	} else {
		return line.substr(0, column_pos);
	}
}

instruction* parser::extract_instruction(string line) {
	// skip label
	size_t column_pos = line.find(":");
	if (column_pos != string::npos) {
		line = line.substr(column_pos + 1);
	}

	string word;

	istringstream iss(line, istringstream::in);
	vector<string> inst;

	while (iss >> word) {
		size_t comma_pos = word.find(",");
		if (comma_pos != string::npos) {
			word = word.substr(0, comma_pos);
		}

		inst.push_back(word);
	}

	switch(inst.size()) {
		case 1: return new instruction(inst[0]);
		case 2: return new instruction(inst[0], inst[1]);
		case 3: return new instruction(inst[0], inst[1], inst[2]);
		default: return NULL;
	}
}


vector<block*> parser::get_code_blocks() {
	return code_blocks;
}

unordered_map<string, int> parser::get_label_dic() {
	return label_dic;
}
