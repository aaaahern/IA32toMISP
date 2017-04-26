#include <iostream>
#include <string.h>
#include <fstream>  
#include "parser.h"
#include "translator.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Usage: IA32toMISP path_to_input path_to_output" << endl;
        return -1;
    }
    // TODO transfer all upper-case letters

    string input_file_path(argv[1]);
    parser parser(input_file_path);
    translator translator;
    string output = translator.translate_IA32_to_MIPS(parser);

    // TODO write output to argv[2];
    string output_file_path(argv[2]);
    ofstream os(output_file_path);  
    if (!os) { 
        std::cerr<<"Error writing to " << argv[2] <<std::endl; 
    } else {  
      os << output;
    }  
    return 0;
}
