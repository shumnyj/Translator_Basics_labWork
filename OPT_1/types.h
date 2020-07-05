#pragma once
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <iostream>	
#include <iomanip>
#include <fstream>
#define TLEN 300 

typedef std::map<short, std::string> tToken;

struct tTreeN
{
	int code;
	std::string name;
	std::list<tTreeN*> children;
};

typedef std::list<tTreeN*>::iterator ttIter;