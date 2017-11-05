#include "llvm/ADT/STLExtras.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <memory>

// 二項演算子の優先度
std::map<char, int> BinopPrecedence;
int GetTokPrecedence();
