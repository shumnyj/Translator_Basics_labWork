#include "SyntaxAn.h"

/*tToken KeywordsTab;
tToken SeparatorsTab;
tToken ConstTab;
tToken IdentifiersTab;
//short int (*TokenString)[3][300];
ts *TokenStr;*/

using namespace std;

extern short int SymbolCategories[128];
extern tToken KeywordsTable;
extern tToken SeparatorsTable;
extern tToken ConstTable;
extern tToken IdentifiersTable;
extern short int TokenString[TLEN][3];

//extern bool SyntaxErr;

int TokensCount;
int TokenIndex;
int errindex;
string listingPath;
ofstream ls;
ostream *resf;
ifstream src;

//int Syntax(int len, short int *sym, ts* tokens, tToken &sep, tToken &keyw, tToken &ident, tToken &con)
tTreeN Parser(int len, string sourcePath, bool &SR, ostream &ost)
{
	tTreeN TokenTree;
	TokenTree.name = "<signal-program>";
	TokenIndex = 0;
	TokensCount = len;
	
	listingPath = sourcePath;
	listingPath.resize(listingPath.size() - 4);
	listingPath += "_ls.txt";
	ls.open(listingPath);
	src.open(sourcePath);
	if (src.is_open() && ls.is_open())
	{
		resf = &ost;
		TokenIndex = _program(TokenIndex, &TokenTree, 1);
		if (TokenIndex == TokensCount)
		{
			cout << "Successful syntax analysis finish" << endl << endl;
			SR = false;
		}
		else
		{
			cout << "Error during syntax analysis" << endl;
			SR = true;
		}

		src.close();
		ls.close();
	}
	else
		cout << "File is not opened/not found" << endl << endl;
	return TokenTree;
}

void lsOutHelper(int index, int tab)
{
	int i;
	string a, errm;
	char skip[256];
	/*char c;
	if (TokenString[index][2] == TokenString[index + 1][2])
		src.get(a, TokenString[index + 1][1] - TokenString[index][1]+1);
	else
	{
		src.getline(a, 256);
	}
	do {
		src.get(c);
	} while ((c >= 8 && c <= 13) || c == 32 && !src.eof());
	src.unget();*/
	if (index == -5)
	{
		errm += "coordinates: " + to_string(TokenString[errindex][1]) + ' ' + to_string(TokenString[errindex][2]);
		errm = " ---ERROR---   Tokens after the end of program                   \n";
		a += errm;
		for (i = 1;i < TokenString[errindex][2] ;i++)
			src.getline(skip, 256);
		//src.get(skip, TokenString[errindex][1]);
		if (TokenString[errindex + 1][2] == TokenString[errindex][2])
			src.get(skip, TokenString[errindex + 1][1] - TokenString[errindex][1] + 1);
		else
			src.getline(skip, 256);
		a += skip;
		while (!src.eof())
		{
			src.getline(skip, 256);
			a += skip;
		}
		*resf << errm << endl;
	}
	else if (index < 0)
	{
		for (i = 1;i < TokenString[errindex][2];i++)
			src.getline(skip, 256);
		src.get(skip, TokenString[errindex][1]);
		if(TokenString[errindex + 1][2] == TokenString[errindex][2] )
			src.get(skip, TokenString[errindex + 1][1] - TokenString[errindex][1] + 1);
		else
			src.getline(skip, 256);
		a = skip;
		errm += "coordinates: " + to_string(TokenString[errindex][1]) + ' ' + to_string(TokenString[errindex][2]);
		errm += " ---ERROR---   Unexpected token                                \n";
		a += errm;
		*resf << errm << endl;
	}
	else if ( index >= TokensCount)
	{
		errm = "coordinates: " + to_string(TokenString[errindex][1]) + ' ' + to_string(TokenString[errindex][2]);
		errm += " ---ERROR---   Unexpected end of file, syntax unfinished        \n";
		a += errm;
		*resf << errm << endl;
	}
	else if (TokenString[index][0] >= 0 && TokenString[index][0] <= 256 && SymbolCategories[TokenString[index][0]] != 6)
		a = TokenString[index][0];
	else if (TokenString[index][0] >= 300 && TokenString[index][0] < 400)
		a = SeparatorsTable[TokenString[index][0]];
	else if (TokenString[index][0] >= 400 && TokenString[index][0] < 500)
		a = KeywordsTable[TokenString[index][0]];
	else if (TokenString[index][0] >= 500 && TokenString[index][0] < 1000)
		a = ConstTable[TokenString[index][0]];
	else if (TokenString[index][0] >= 1000)
		a = IdentifiersTable[TokenString[index][0]];
	else if (TokenString[index][0] == -2)
	{
		errm = "coordinates: " + to_string(TokenString[index][1]) + ' ' + to_string(TokenString[index][2]);
		errm += " ---ERROR---   Unexpected end of file (opened comment)          \n";
		a += errm;
		*resf <<  errm << endl;
	}
	else 
	{
		for (i = 1;i < TokenString[index][2];i++)
			src.getline(skip, 256);
		src.get(skip, TokenString[index][1]);
		if (TokenString[errindex + 1][2] == TokenString[errindex][2])
			src.get(skip, TokenString[index + 1][1] - TokenString[index][1] + 1);
		else
			src.getline(skip, 256);
		a = skip;
		errm = "coordinates: " + to_string(TokenString[index][1]) + ' ' + to_string(TokenString[index][2]);
		errm += " ---ERROR---   Unknown token                                   \n";
		a += errm;
		*resf <<  errm << endl;
	}
	ls << setw(tab * 4) << ' ' << a << endl;
	
}

tTreeN* TreeAdd(string name, tTreeN *par, int code)
{
	tTreeN *x;
	x = new tTreeN;
	x->name = name;
	x->code = code;
	par->children.push_back(x);
	return x;
}

void TestTreeOut(tTreeN *par, std::ostream &f, std::list<tTreeN*> *x)
{
	if (x == NULL)
		x = new std::list<tTreeN*>;		//list to track markdown
	x->push_back(par);
	auto it1 = x->begin();
	auto it2 = x->begin();
	it2++;

	while (it2 != x->end())
	{
		if((*it1)->children.back() == (*it2) && (*it2) != x->back())
			f << setw(3) << " ";
		else
			f << setw(3) << "|";
		it1++;
		it2++;
	}
	/*for (int i = 0;i < depth;i++)
	{
		//if(i==depth-2 && (*it) == par->children.back() && (*it)->children.empty())
		//	cout << setw(3) << " ";
		//else
		cout << setw(3) << "|";
	}*/
	f << "_" << par->name << endl;
	for (auto it = par->children.begin(); it != par->children.end(); it++)
	{
		TestTreeOut((*it), f, x);
	}
	x->pop_back();
}

int _program(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<signal-program>" << endl;
	par = TreeAdd("<program>", par);
	//par = par->children.front();
	ls << setw(++tab * 4) << ' ' << "<program>" << endl;
	lsOutHelper(index, tab+1);
	if (TokenString[index][0] == 401)	//401 = "PROGRAM"
	{
		TreeAdd("PROGRAM", par, 401);
		index = _procident(index + 1, par, tab+1);
		lsOutHelper(index, tab + 1);
		if (index >= 0 && TokenString[index][0] == (int)';') //59
		{
			TreeAdd(";", par, ';');
			index = _block(index + 1, par, tab + 1);
			if (index >= 0 && TokenString[index][0] == (int)'.')
			{
				TreeAdd(".", par, '.');
				lsOutHelper(index, tab + 1);
				index++;
				if (index > 0 && (TokenString[index][0] != 0 || TokenString[index][1] != 0 || TokenString[index][2] != 0))
				{
					errindex = index;
					TreeAdd("<---ERROR--->", par);
					lsOutHelper(-5, 0); //file is longer than should be
				}
				return index;
			}
		}
	}
	if (index >= 0)
	{
		TreeAdd("<---ERROR--->", par);
		lsOutHelper(index, tab + 1);
	}
	return -1;
}

int _block(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<block>" << endl;
	par = TreeAdd("<block>", par);
	index = _vardec(index, par, tab + 1);
	lsOutHelper(index, tab + 1);
	if (index > 0 && TokenString[index][0] == 402) // 402 = "BEGIN"
	{
		TreeAdd("BEGIN", par, 402);
		index = _stlist(index + 1, par, tab + 1);
		//lsOutHelper(index, tab + 1);
		if (index > 0 && TokenString[index][0] == 403) // 403 = "END"
		{
			lsOutHelper(index, tab + 1);
			TreeAdd("END", par, 403);
			return index + 1;
		}
		TreeAdd("<---ERROR--->", par);
		lsOutHelper(index, tab + 1);
	}
	TreeAdd("<---ERROR--->", par);
	return -4;
}

int _vardec(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<variable-declarations>" << endl;
	par = TreeAdd("<variable-declarations>", par);
	lsOutHelper(index, tab + 1);
	if (TokenString[index][0] == 404) //404 = "VAR"
	{
		TreeAdd("VAR", par, 404);
		index = _declist(index + 1, par, tab + 1);
		return index;
	}
	TreeAdd("<--empty-->", par);
	return index; //empty
}

int _declist(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<declarations-list>" << endl;
	par = TreeAdd("<declarations-list>", par);
	int x = index;
	/*do
	{
		rec = index;
		index = _dec(index, par, tab + 1);
	} while (index >= 0 && rec != index);
	par->children.pop_back();	// remove last *undexisting* declaration branch;
	if (x == rec)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
	}*/
	index = _dec(index, par, tab + 1);
	if (index != x)
	{
		index = _declist(index, par, tab + 1);
	}
	else if (index >=0)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
	}
	return index; //return pointer after last decl or pass error from declarations
}

int _dec(int index, tTreeN *par, int tab)
{
	auto x = ls.tellp();
	tTreeN *old;
	old = par;
	ls << setw(tab * 4) << ' ' << "<declaration>" << endl;
	par = TreeAdd("<declaration>", par);
	int rec = index;
	index = _varident(index, par, tab + 1);
	if (index >= 0) 
	{
		lsOutHelper(index, tab + 1);
		if (TokenString[index][0] == (int)':')
		{
			TreeAdd(":", par, ':');
			index = _attr(index + 1, par, tab + 1);
			if (index >= 0)
			{
				index = _attrlist(index, par, tab + 1);
				lsOutHelper(index, tab + 1);
				if(index >= 0 && TokenString[index][0] == (int)';')
					TreeAdd(";", par, ';');
					return index + 1;
			}
		}
		TreeAdd("<--ERROR-->", par);
		lsOutHelper(index, tab + 1);
		if(index >= 0)
			errindex = index;
		return -1; // no ':' or no ';' or no attributes
	}
	else
	{ 
		old->children.clear(); // remove last *unexisting* declaration branch;
		ls.seekp(x);
		return rec; //not variable -> empty
	}
}
int _attrlist(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<attributes-list>" << endl;
	par = TreeAdd("<attributes-list>", par);
	int  x = index;
	index = _attr(index, par, tab + 1);
	if (index < 0)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
		index = x;
	}
	else
	{
		index = _attrlist(index, par, tab + 1);
	}
	/*do
	{
		rec = index;
		index = _attr(index, par, tab + 1);
	} while (index >= 0 && rec != index);//rec==index -> no more attributes
	if (x == rec)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
	}*/
	return index; // _attr -> -1 => end of attrlist 
}

int _attr(int index, tTreeN *par, int tab)
{
	// SIGNAL | INTEGER | FLOAT | EXT
	if (TokenString[index][0] >= 405 && TokenString[index][0] <= 408)
	{
		ls << setw(tab * 4) << ' ' << "<attribute>" << endl;
		par = TreeAdd("<attribute>", par);
		TreeAdd(KeywordsTable[TokenString[index][0]], par, TokenString[index][0]);
		lsOutHelper(index, tab + 1);
		return index + 1;
	}
	else
	{
		//ls << setw(tab * 4) << ' ' << "<ERROR>" << endl;
		errindex = index;
		return -3;
	}
}

int _stlist(int index, tTreeN *par, int tab)
{

	int  x = index;
	ls << setw(tab * 4) << ' ' << "<statements-list>" << endl;
	par = TreeAdd("<statements-list>", par);
	index = _st(index, par, tab + 1);
	if (index == x)
	{ 
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
		index = x;
	}
	else if (index >= 0)
	{
		index = _stlist(index, par, tab + 1);
	}
	/*do
	{
		rec = index;
		index = _st(index, par, tab + 1);
	} while (index >= 0 && rec != index);
	if (x == rec)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
	}*/
	return index;
}

int _st(int index, tTreeN *par, int tab)
{
	if (TokenString[index][0] == 409) //409 = "FOR"
	{				
		ls << setw(tab * 4) << ' ' << "<statement>" << endl;
		par = TreeAdd("<statement>", par);
		TreeAdd("FOR", par, 409);
		lsOutHelper(index, tab + 1);
		index = _varident(index + 1, par, tab + 1);
		lsOutHelper(index, tab + 1);
		if (index >= 0 && TokenString[index][0] == 301) // 301 = ":="
		{
			TreeAdd(":=", par, 301);
			index = _loop(index + 1, par, tab + 1);
			lsOutHelper(index, tab + 1);
			if (index >= 0 && TokenString[index][0] == 410) // 410 = "ENDFOR"
			{
				TreeAdd("ENDFOR", par, 410);
				index++;
				lsOutHelper(index, tab + 1);
				if (index >= 0 && TokenString[index][0] == (int)';')
					TreeAdd(";", par, ';');
					return index + 1; //working statement ->  return pointer to next lexem
			}
		}
		TreeAdd("<---ERROR--->", par);
		errindex = index;
		return -4; //error inside of statement
	}
	return index; // no next statement
}

int _loop(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<loop-declaration>" << endl;
	par = TreeAdd("<loop-declaration>", par);
	index = _expr(index, par, tab + 1);
	lsOutHelper(index, tab + 1);
	if (index >= 0 && TokenString[index][0] == 411) //411 = "TO"
	{
		TreeAdd("TO", par, 411);
		index = _expr(index + 1, par, tab + 1);
		lsOutHelper(index, tab + 1);
		if (index >= 0 && TokenString[index][0] == 412) //412 = "DO"
		{
			TreeAdd("DO", par, 412);
			index = _stlist(index + 1, par, tab + 1);
			return index;
		}
	}
	TreeAdd("<---ERROR--->", par);
	return -4; //error in loop declaration 
}

int _expr(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<expression>" << endl;
	par = TreeAdd("<expression>", par);
	index = _mult(index, par, tab + 1);
	if (index >= 0)
		index = _multlist(index, par, tab + 1);
	return index;//-1 if _mult returned -1
}

int _multlist(int index, tTreeN *par, int tab)
{
	int x = index;
	ls << setw(tab * 4) << ' ' << "<multipliers-list>" << endl;
	par = TreeAdd("<multipliers-list>", par);
	index = _multinst(index, par, tab + 1);
	if (index > x)
	{
		index = _mult(index, par, tab + 1);
		if (index < 0)
			return -1;
		else
			index = _multlist(index, par, tab + 1);
		
	}
	else if (index < 0)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);


		index = x;
	}
	/*do {
		rec = index;
		index = _multinst(index, par, tab + 1);
		if (index >= 0)
		{
			index = _mult(index, par, tab + 1);
			if (index < 0)
				return -1; //if there is instruction but no multiplier
		} // else it might be empty -> return rec 
	} while (index >= 0 && rec != index);
	if (x == rec)
	{
		ls << setw(++tab * 4) << ' ' << "<--empty-->" << endl;
		TreeAdd("<--empty-->", par);
	}*/
	return index;
}
int _multinst(int index, tTreeN *par, int tab)
{
	string x;
	if (TokenString[index][0] == (int)'*' || TokenString[index][0] == (int)'/' ||
		TokenString[index][0] == (int)'&' || TokenString[index][0] == 413) // 413="MOD"
	{
		ls << setw(tab * 4) << ' ' << "<multiplication-instruction>" << endl;
		par = TreeAdd("<multiplication-instruction>", par);
		if (TokenString[index][0] == 413)
			TreeAdd("MOD", par, 413);
		else
		{
			x += char(TokenString[index][0]);
			TreeAdd(x, par, TokenString[index][0]);
		}
		lsOutHelper(index, tab + 1);
		return index + 1;
	}
	else
	{
		//ls << setw(tab * 4) << ' ' << "<ERROR>" << endl;
		errindex = index;
		return -4;
	}
}

int _mult(int index, tTreeN *par, int tab)
{
	if (TokenString[index][0] >= 501 && TokenString[index][0] <= 1000) //constant table
	{
		ls << setw(tab * 4) << ' ' << "<multiplier>" << endl;
		par = TreeAdd("<multiplier>", par);
		ls << setw(++tab*4) << ' ' << "<unsigned-integer>" << endl;
		par = TreeAdd("<unsigned-integer>", par);
		TreeAdd(ConstTable[TokenString[index][0]], par, TokenString[index][0]);
		lsOutHelper(index, tab + 1);
		return index + 1;
	}
	else
	{
		ls << setw(tab * 4) << ' ' << "<multiplier>" << endl;
		par = TreeAdd("<multiplier>", par);
		index = _varident(index, par, tab + 1);
		return index; //err if not identifier
	}
}

int _ident(int index, tTreeN *par, int tab)
{
	if (TokenString[index][0] > 1000)
	{
		ls << setw(tab * 4) << ' ' << "<identifier>" << endl;
		par = TreeAdd("<identifier>", par);
		TreeAdd(IdentifiersTable[TokenString[index][0]], par, TokenString[index][0]);
		lsOutHelper(index, tab + 1);
		return index + 1;
	}
	else
	{
		//ls << setw(tab * 4) << ' ' << "<ERROR>" << endl;
		//lsOutHelper(index, tab + 1);
		//TreeAdd("<---ERROR--->", par);
		errindex = index;
		return -3;
	}
}

int _procident(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<procedure-identifier>" << endl;
	par = TreeAdd("<procedure-identifier>", par);
	index = _ident(index, par, tab + 1);
	return index;
}

int _varident(int index, tTreeN *par, int tab)
{
	ls << setw(tab * 4) << ' ' << "<variable-identifier>" << endl;
	par = TreeAdd("<variable-identifier>", par);
	index = _ident(index, par, tab + 1);
	return index;
}