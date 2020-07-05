#pragma once
#include "types.h"

//#define UN 300		//length of massive reserved for unique check

void Generator(tTreeN *tree, std::ostream &ost = std::cout, std::ostream &r = std::cout);
int SPR(tTreeN *node);
bool CheckUnique(int code);