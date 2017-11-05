#include "common.h"
#include "lexer.h"

static int gettok() {
    static int LastChar = ' ';

    while (isspace(LastChar))
        LastChar = getchar();

    // MARK: - Identifier
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def") return tok_def;
        if (IdentifierStr == "extern") return tok_extern;
        return tok_identifier;
    }

    // MARK: - Number
    bool DotFlag = false;
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            DotFlag = LastChar == '.';
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || (LastChar == '.' && !DotFlag));

        if (LastChar == '.') {
            // error
            return tok_eof;
        }

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    // MARK: - Comment
    if (LastChar == '#') {
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // MARK: - EOF
    if (LastChar == EOF)
        return tok_eof;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int getNextToken() {
    return CurTok = gettok();
}
