#include "CodeGen.h"

using namespace std;

extern short int SymbolCategories[128];
extern tToken KeywordsTable;
extern tToken SeparatorsTable;
extern tToken ConstTable;
extern tToken IdentifiersTable;

string assembled;
string buffer;
string reserve;
string errval;
int IdUnq[TLEN] = { 0 };
int labelNum = 0;
ostream *res;
bool u = false;

void Generator(tTreeN *tree, ostream &ost, ostream &r)
{
	tTreeN *current = tree;
	if (current->name == "<signal-program>" && current->children.size()== 1)
	{
		current = current->children.front();
		if (current->name == "<program>")
		{
			res = &r;
			SPR(current);
			ost << assembled << endl;
			cout << "Code generation is finished" << endl;
		}
	}
}

bool CheckUnique(int code)
{
	int i;
	for (i = 0; i < TLEN; i++)
	{
		if (IdUnq[i] == code)
			return false;
		else if (IdUnq[i] == 0)
		{
			IdUnq[i] = code;
			return true;
		}
	}
	return true;
}

int SPR(tTreeN *node)
{
	tTreeN *current = node;
	ttIter it;
	int savepoint, tmp;
	it = current->children.begin(); //start of children list

	if(current->name == "<program>")
	{
		it++;
		SPR((*it));		//<procedure-identifier>
		it++;
		it++;
		SPR((*it));		//<block>
	}
	else if (current->name == "<procedure-identifier>")
	{
		tmp = SPR((*it)); //<identifier>
		if (CheckUnique(tmp))
		{
			assembled += ';' + IdentifiersTable[tmp] + '\n';
			return 1;
		}
		else
		{
			assembled += ';' + IdentifiersTable[tmp] + " !ERROR! Identifier is already reserved\n";
			*res << IdentifiersTable[tmp] << " !ERROR! Identifier is already reserved\n";
			return 0;
		}
	}
	else if (current->name == "<identifier>")
	{
		return (*it)->code;
	}
	else if (current->name == "<block>")
	{
		SPR((*it)); //<variable-declarations>
		it++;
		it++;
		//assembled += "CODE SEGMENT\n ASSUME ds:DATA, cs:CODE\n begin:\n PUSH EBP\n MOV EBP,ESP\n XOR EAX, EAX\n MOV ESP EBP\n POP EBP\n\n";
		assembled += "CODE SEGMENT\n ASSUME ds:DATA, cs:CODE\n begin:\n MOV AX, DATA\n MOV DS,AX\n\n";
		u = false;
		SPR((*it)); //<statements-list>
		assembled += "\n MOV AX, 4c00h\n INT 21h\nCODE ENDS\n END begin\n\n";
	}
	else if (current->name == "<variable-declarations>")
	{
		if ((*it)->name == "<--empty-->")
			return 0;
		else
		{
			u = true;
			assembled += "DATA SEGMENT\n";
			it++;
			tmp = SPR((*it));		 //<declarations-list>
			assembled += "DATA ENDS\n\n";
			u = false;
			if (tmp == 0)
				return 0;
		}
		return 1;
	}
	else if (current->name == "<declarations-list>")
	{
		if ((*it)->name == "<--empty-->")
			return 0;
		else
		{
			SPR((*it));		//<declaration>
			it++;
			SPR((*it));		//<declarations-list>
		}
	}
	else if (current->name == "<declaration>")
	{
		tmp = SPR((*it));		//<variable-identifier>
		if (tmp != 0)
			assembled += ' ' + IdentifiersTable[tmp] + " dd ?\n";
		else
		{
			assembled += ' ' + errval + " dd ?; !ERROR! identifier " + errval + " is already declared\n";
			*res << "!ERROR! identifier " << errval << " is already declared\n"; 
		}
		buffer.clear();
		return 1;
		/*it++;
		it++;
		SPR((*it));	//<attribute>
		it++;
		SPR((*it)); //<attributes-list>*/
	}
	else if (current->name == "<variable-identifier>")
	{
		tmp = SPR((*it));	//<identifier>
		if (CheckUnique(tmp) == u) //true for DATA; false for CODE
			return tmp;
		else
		{
			errval = IdentifiersTable[tmp];
			return 0;
			/*if (u)
				buffer +="; !ERROR! Identifier " + IdentifiersTable[tmp] + " is already reserved\n";
			else
				buffer +="; !ERROR! Identifier " + IdentifiersTable[tmp] + " is undeclared\n";*/
		}
		
	}
	else if (current->name == "<statements-list>")
	{
		if ((*it)->name == "<--empty-->")
		{
			assembled += " NOP\n";
			return 0;
		}
		else
		{
			SPR((*it));		//<statement>
			it++;
			SPR((*it));		//<statements-list>
			return 1;
		}
	}
	else if (current->name == "<statement>")
	{

		it++;
		tmp = SPR((*it));
		reserve = " MOV " + IdentifiersTable[tmp] + ",EAX\n MOV EAX,EBX\n";
		reserve += " CMP EAX," + IdentifiersTable[tmp] + "\n JL ?L" + to_string(labelNum) + "\n";
		labelNum++;
		reserve += " MOV EDX,1\n";
		reserve += " SUB EAX, " + IdentifiersTable[tmp] + "\n INC EAX\n MOV ECX, EAX\n";
		reserve += "?L" + to_string(labelNum) + ": NOP\n";
		savepoint = labelNum;
		it++;
		it++;
		SPR((*it));		//<loop-declaration>
		
		assembled += "\n MOV EAX, " + IdentifiersTable[tmp] + "\n DEC EAX\n MOV " + IdentifiersTable[tmp] + ",EAX\n";
		assembled += " LOOP ?L" + to_string(savepoint) + "\n?L" + to_string(savepoint - 1) + ": NOP\n\n";
		labelNum++;
		//it++;

	}
	else if (current->name == "<loop-declaration>")
	{
		SPR((*it));		//<expression>
		assembled += " MOV EBX, EAX; bottom\n"; //lowbound -> EBX
		it++;
		it++;
		SPR((*it));		//<expression>
		assembled += " XCHG EBX, EAX ; top\n\n";//lowbound -> EAX, highbound->EBX;
		assembled += reserve;
		reserve.clear();
		it++;
		it++;
		SPR((*it));		//<statements-list>
		return 1;
	}
	else if (current->name == "<expression>")
	{
		buffer = " MOV EAX,";
		SPR((*it));			//<multiplier>
		buffer += "\n";
		it++;
		SPR((*it));			//<multipliers-list>
		assembled += buffer;
		buffer.clear();
	}
	else if (current->name == "<multiplier>")
	{
		tmp = SPR((*it));		
		if (tmp > 500 && tmp < 1000)
			buffer += ConstTable[tmp];
		else if (tmp > 1000)
			buffer += IdentifiersTable[tmp];
		else
		{
			buffer += errval + " ;!ERROR! variable " + errval + " is not declared";
			*res << "!ERROR! variable " << errval << " is not declared";
		}
		return tmp;
	}
	else if (current->name == "<unsigned-integer>")
	{
		return (*it)->code;
	}
	else if (current->name == "<multipliers-list>")
	{
		if ((*it)->name == "<--empty-->")
		{
			buffer += " NOP ;empty\n";
			return 0;
		}
		else
		{
			tmp = SPR((*it));		//<multiplication-instruction>
			if (tmp == 413)
			{
				buffer += " IDIV ";
				it++;
				SPR((*it));		//<multiplier>
				buffer += "\n MOV EAX, EDX\n";
			}
			else
			{
				if (tmp == '*')
					buffer += " IMUL EAX,";
				else if (tmp == '/')
					buffer += " IDIV ";
				else if (tmp == '&')
					buffer += " AND EAX,";
				it++;
				SPR((*it));		//<multiplier>
				buffer += "\n";
			}
			//assembled += buffer;
			//buffer.clear();
			it++;
			SPR((*it));		//<multipliers-list>
			return 1;
		}
	}
	else if (current->name == "<multiplication-instruction>")
	{
		return (*it)->code;
	}
	return -1;
}