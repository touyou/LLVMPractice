#include "common.h"
#include "ast.h"
#include "parser.h"

int main() {
    fprintf(stderr, "ready> ");
    getNextToken();

    MainLoop();

    return 0;
}
