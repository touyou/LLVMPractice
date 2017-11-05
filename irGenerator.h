#include "common.h"

std::unique_ptr<Module> TheModule;
IRBuilder<> Builder(getGlobalContext());
std::map<std::string, std::unique_ptr<Value>> NamedValues;
