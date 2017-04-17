#include <iostream>
#include <string.h>
#include "parser.h"
#include "translator.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ia32";
        return -1;
    }
    // TODO transfer all upper-case letters

    string input_file_path(argv[1]);
    parser parser(input_file_path);
    translator translator;
    string output = translator.translate_IA32_to_MIPS(parser);

    // TODO write output to argv[2];

    return 0;
}
