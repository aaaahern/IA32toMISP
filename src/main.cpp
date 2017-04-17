#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ia32";
        return -1;
    }
    parser parser = new parser(argv[1]);
    return 0;
}
