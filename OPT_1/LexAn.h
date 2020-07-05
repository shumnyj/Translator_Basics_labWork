#pragma once

#include "types.h"
#include <iostream>



/*struct tToken
{
	short int num;
	const char* name;
	tToken* next;
};*/
/*struct tLexem
{
	short int num;
	short int top;
	short int left;
};*/

void FillSymCat(short int *tab);
tToken FillKeyList();
tToken FillSepList();

void PrintLexStr(int len, short int(*tokens)[3]);
void PrintLexTable(tToken tab);
void TestTableOut(int len, short int(*tokens)[3], tToken ident, tToken con);
/*
	Function for checking and/or adding token to the token tables
	Returns token index in respective table
*/
int TableChkAdd(tToken &tab, std::string token);		//short start = 501

/*Overloaded TableChkAdd for checking identifier-type tokens*/
int TableChkAdd(tToken &tab1, tToken &tab2, std::string token);

/*
	Returns token string length
	sym - approved symbol table
	tokens - empty token string; element = mas[3]
	sep - multicharacter separators table
	keyw - keywords  table
	ident - identifiers table
	con - number constants table
	fname - source filename
*/
int Scanning(short int *sym, short int(*tokens)[3], tToken &sep, tToken &keyw, tToken &ident, tToken &con, std::string fname, std::ostream &ost = std::cout);