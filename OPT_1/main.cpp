// Course/lab work
// Translator for subset of SIGNAL language constructs
// Created by Troian Borys KV-62

/*
Variant 22
1. <signal-program> -->		<program>
2. <program> -->			PROGRAM <procedure-identifier> ; <block>.
3. <block> -->				<variable-declarations> BEGIN <statements-list> END
4. <variable-declarations> -->	VAR <declarations-list> | <empty>
5. <declarations-list> -->		<declaration><declarations-list> | <empty>
6. <declaration> -->			<variable-identifier> : <attribute><attributes-list> ;
7. <attributes-list> -->		<attribute> <attributes-list> | <empty>
8. <attribute> -->				SIGNAL | INTEGER | FLOAT | EXT
9. <statements-list> -->		<statement> <statements-list> | <empty>
10. <statement> -->				FOR <variable-identifier> := <loop-declaration> ENDFOR ;
11. <loop-declaration> -->		<expression> TO <expression> DO <statements-list>
12. <expression> -->			<multiplier><multipliers-list>
13. <multipliers-list> -->		<multiplication-instruction> <multiplier><multipliers-list> | <empty>
14. <multiplication-instruction> -->	* | / | & | MOD
15. <multiplier> -->			<variable-identifier> | <unsigned-integer>
16. <variable-identifier> -->	<identifier>
17. <procedure-identifier> -->	<identifier>
18. <identifier> -->			<letter><string>
19. <string> -->				<letter><string> | <digit><string> |<empty>
20. <unsigned-integer> -->		<digit><digits-string>
21. <digits-string> -->			<digit><digits-string> | <empty>
22. <digit> -->			0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
23. <letter> -->		A | B | C | D | ... | Z
*/
#include "LexAn.h"
#include "SyntaxAn.h"
#include "CodeGen.h"

short int SymbolCategories[128];
tToken KeywordsTable;
tToken SeparatorsTable;
tToken ConstTable;
tToken IdentifiersTable;
bool SyntaxErr = false;

short int TokenString[TLEN][3];
tTreeN TokenTree;
std::string resultPath;
std::ofstream resf;

int main()
{	
	using namespace std;
	string sourcePath;
	string asmPath;
	int len = 0;
	
	cout << "Enter source file path/name" << endl;
	cin >> sourcePath;
	getchar();
	if (!sourcePath.empty())
	{
		resultPath = sourcePath.substr(0, sourcePath.length() - 4);
		resultPath += "_res.txt";
		resf.open(resultPath);
	}
	else
	{
		cout << "Invalid source input" << endl;
		return 0;
	}

	FillSymCat(SymbolCategories);
	KeywordsTable = FillKeyList();
	SeparatorsTable = FillSepList();

	cout << "Scanning the file" << endl;
	resf << "Lexic analysis:" << endl;
	len = Scanning(SymbolCategories, TokenString, SeparatorsTable, KeywordsTable, IdentifiersTable, ConstTable, sourcePath, resf);
	if (len > 0)
	{
		TestTableOut(len, TokenString, IdentifiersTable, ConstTable);
		cout << "Parsing" << endl;
		resf << "Syntax analysis:" << endl;
		TokenTree = Parser(len, sourcePath, SyntaxErr, resf);
		ofstream tf("tree.txt");
		TestTreeOut(&TokenTree, tf);
		tf.close();
	}

	if (!SyntaxErr)
	{
		asmPath = sourcePath.substr(0, sourcePath.length() - 4) + ".asm";
		ofstream as(asmPath);
		cout << "Generating code" << endl;
		resf << "Semantic analysis:" << endl;
		Generator(&TokenTree, as, resf);
		as.close();
		cout << "Done" << endl;
	}
	resf.close();
	getchar();
	return 0;
}