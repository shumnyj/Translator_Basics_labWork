/*
Варіант 22
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
#pragma once
#include "types.h"

#include <iomanip>

//typedef short ts[TLEN][3];

tTreeN Parser(int len, std::string sourcePath, bool &SR, std::ostream &ost = std::cout);
void lsOutHelper(int index, int tab);
tTreeN* TreeAdd(std::string name, tTreeN *par, int code = 0);
void TestTreeOut(tTreeN *tree, std::ostream &f, std::list<tTreeN*> *x = NULL);

int _program(int index,  tTreeN *par, int tab = 0);
int _block(int index,  tTreeN *par, int tab = 0);
int _vardec(int index,  tTreeN *par, int tab = 0);
int _declist(int index,  tTreeN *par, int tab = 0);
int _dec(int index,  tTreeN *par, int tab = 0);
int _attrlist(int index,  tTreeN *par, int tab = 0);
int _attr(int index,  tTreeN *par, int tab = 0);
int _stlist(int index,  tTreeN *par, int tab = 0);
int _st(int index,  tTreeN *par, int tab = 0);
int _loop(int index,  tTreeN *par, int tab = 0);
int _expr(int index,  tTreeN *par, int tab = 0);
int _multlist(int index,  tTreeN *par, int tab = 0);
int _multinst(int index,  tTreeN *par, int tab = 0);
int _mult(int index,  tTreeN *par, int tab = 0);
int _ident(int index,  tTreeN *par, int tab = 0);
int _varident(int index, tTreeN *par, int tab = 0);
int _procident(int index, tTreeN *par, int tab = 0);
