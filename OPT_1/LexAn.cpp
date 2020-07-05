#include "LexAn.h"
//extern std::string resultPath;

int Scanning(short int *sym, short int (*tokens)[3], tToken &sep, tToken &keyw, tToken &ident, tToken &con, std::string fname, std::ostream &ost)
{
	int top=1, left=1, i=0;
	
	char c;
	using namespace std;
	string buffer;
	ifstream codefile;
	codefile.open(fname);
	if (codefile.is_open() == true)
	{
		codefile.get(c);
		while (!codefile.eof())
		{
			switch (sym[(int)c])
			{
			case 0:
				do {
					if (c == '\n')
					{
						top++;
						left = 1;
					}
					else
						left++;
					codefile.get(c);
				} while (sym[(int)c] == 0 && !codefile.eof());
				break;
			case 1:
				do {
					//left++;
					buffer += c;
					codefile.get(c);
				} while (sym[(int)c] == 1 && !codefile.eof());
				if (sym[(int)c] == 2 || sym[(int)c] == 6)
				{
					do {
						buffer += c;
						codefile.get(c);
					} while ((sym[(int)c] == 2 || sym[(int)c] == 6) && !codefile.eof());
					tokens[i][0] = -1;
					tokens[i][1] = left;
					tokens[i][2] = top;
					ost << "Unidentified token on position " << left << ' ' << top << '\n';	//err wrong sym
				}
				else
				{
					tokens[i][0] = TableChkAdd(con, buffer);
					tokens[i][1] = left;
					tokens[i][2] = top;
				}
				left += buffer.length();
				buffer.clear();
				i++;
				break;
			case 2:
				buffer += c;
				//left++;
				codefile.get(c);
				while ((sym[(int)c] == 1 || sym[(int)c] == 2) && !codefile.eof())
				{
					buffer += c;
					//left++;
					//codefile >> c;
					codefile.get(c);
				}

				tokens[i][0] = TableChkAdd(keyw, ident, buffer);
				tokens[i][1] = left;
				tokens[i][2] = top;
				left += buffer.length();
				buffer.clear();
				i++;
				break;
			case 3:
				tokens[i][0] = int(c);
				tokens[i][1] = left;
				tokens[i][2] = top;
				left++;
				i++;
				codefile.get(c);
				break;
			case 41:
				//left++;
				codefile.get(c);
				if (c == '=')		//спрощено по причині 1 випадку
				{
					tokens[i][0] = TableChkAdd(sep, ":=");
					tokens[i][1] = left;
					tokens[i][2] = top;
					left += 2;
					codefile.get(c);
				}
				else
				{
					tokens[i][0] = (int)':';
					tokens[i][1] = left;
					tokens[i][2] = top;
					left++;
				}
				i++;
				//buffer.clear();
				break;
			case 5:
				//left++;
				codefile.get(c);
				if (c == '*')
				{
					do {
						left++;
						do {
							codefile.get(c);
							if (c == '\n')
							{
								left = 1;
								top++;
							}
							else
								left++;
						} while (c != '*' && !codefile.eof());
						if (codefile.eof())
						{
							tokens[i][0] = -2;
							tokens[i][1] = left;
							tokens[i][2] = top;
							ost << "Unexpected end of file on position " << left << ' ' << top << "\n ";	//err eof
							//ost << " Terminating process\n";	//err eof
							i++;
							break;
						}
						codefile.get(c);
					} while (c != ')');
					if (c == ')')
					{
						left++;
						codefile.get(c);
					}
				}
				else
				{
					tokens[i][0] = -1;
					tokens[i][1] = left;
					tokens[i][2] = top;
					left++;
					ost << "Unidentified token on position " << left << ' ' << top << '\n';	//err wrong sym
					codefile.get(c);
				}
				break;
			case 6:
				tokens[i][0] = -1;
				tokens[i][1] = left;
				tokens[i][2] = top;
				ost << "Unidentified token on position " << left << ' ' << top << '\n';	//err wrong sym
				left++;
				i++;
				codefile.get(c);
				break;
			}
		}
		codefile.close();
	}
	else
	{
		cout << "Invalid file input" << '\n';
		return -1;
	}
	
	return i;
}


int TableChkAdd(tToken &tab, std::string token)
{
	tToken::iterator it;
	int n;
	//n = tab.begin()->first;

	if (tab.empty())
	{
		//n = start;
		if (token[0]>47 && token[0] <58)
			n = 501;
		else
			n = 1001;
	}
	else
	{
		for (it = tab.begin(); it != tab.end(); it++)
		{
			if (token.compare(it->second) == 0)
				return it->first;
		}
		n = (tab.rbegin()->first) + 1;
	}	
	tab[n] = token;
	return n;
}

int TableChkAdd(tToken &tab1, tToken &tab2, std::string token)
{
	tToken::iterator it;
	for (it = tab1.begin(); it != tab1.end(); it++)
	{
		if (token.compare(it->second) == 0)
			return it->first;
	}
	return TableChkAdd(tab2, token);
}

void PrintLexStr(int len, short int(*tokens)[3])
{
	using namespace std;
	int i;
	cout << setw(5) << "Num" << setw(4) << "x" << setw(4) << "y" << setw(5) << "Code" << endl;
	for (i = 0;i < len;i++)
	{
		cout << setw(5) << i << setw(4) << tokens[i][1] << setw(4) << tokens[i][2] << setw(5) << tokens[i][0] << endl;
	}
	cout << endl;
}

void PrintLexTable(tToken tab)
{
	using namespace std;
	tToken::iterator it;
	if (tab.empty())
	{
		cout << "No entries\n";
		return;
	}
	else
	{
		cout << setw(5) << "Code" << ' ' << "Token" << endl;
		for (it = tab.begin(); it != tab.end(); it++)
		{
			cout << setw(5) << it->first << ' ' << it->second << endl;
		}
	}
	cout << endl;
}

void TestTableOut(int len, short int(*tokens)[3], tToken ident, tToken con)
{
	PrintLexStr(len, tokens);
	std::cout << "Identifier table:\n";
	PrintLexTable(ident);
	std::cout << "Constant table:\n";
	PrintLexTable(con);
	
}
void FillSymCat(short int *tab)
{
	int i=0;
	for (i = 0;i < 8;i++)
		tab[i] = 6;
	for(i=8;i<14;i++)
		tab[i] = 0;
	for (i = 14;i < 32;i++)
		tab[i] = 6;
	tab[i] = 0;		//32
	for (i = 33;i < 46;i++)
		tab[i] = 6;
	tab[46] = 3;	// 46  .
	tab[47] = 3;	// 47  /
	tab[38] = 3;	// 38 &
	tab[40] = 5;	//40 (
	tab[42] = 3;	// 42 *
	for (i = 48;i < 58;i++)//num
		tab[i] = 1;
	tab[58] = 41;	// 58 :  
	tab[59] = 3;	// 59 ;
	for (i = 60;i < 65;i++)
		tab[i] = 6;
	for (i = 65;i < 91;i++)//let
		tab[i] = 2;
	for (i = 91;i < 128;i++)
		tab[i] = 6;
	return;
}

tToken FillKeyList()
{
	tToken Keywords;
	Keywords[401] = "PROGRAM";
	Keywords[402] = "BEGIN";
	Keywords[403] = "END";
	Keywords[404] = "VAR";
	Keywords[405] = "SIGNAL";
	Keywords[406] = "INTEGER";
	Keywords[407] = "FLOAT";
	Keywords[408] = "EXT";
	Keywords[409] = "FOR";
	Keywords[410] = "ENDFOR";
	Keywords[411] = "TO";
	Keywords[412] = "DO";
	Keywords[413] = "MOD";
	return Keywords;
}

tToken FillSepList()
{
	tToken Separators;
	Separators[301] = ":=";
	return Separators;
}