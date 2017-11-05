// 未知の文字の時は0から255
// すでにわかっているものなら以下のenumで分けられる
enum Token {
    tok_eof = -1,

    tok_def = -2,
    tok_extern = -3,

    tok_identifier = -4,
    tok_number = -5,
};

// グローバルにしているけど後々変えるべき
// namespaceを使うとか？
std::string IdentifierStr;
double NumVal;
int CurTok;

int getNextToken();
