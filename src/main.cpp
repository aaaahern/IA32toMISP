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
    char argv1[sizeof(argv[1])];
    strcpy(argv1, argv[1]);
    string input_file_path(argv1);
    parser parser(input_file_path);
    translator translator;
    string output = translator.translate_IA32_to_MIPS(parser);

    // TODO write output to argv[2];

    return 0;
}
