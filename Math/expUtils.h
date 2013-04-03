#ifndef ENTERED_HERE
#define ENTERED_HERE

#pragma once

#include "expDefines.h"

#define MAXIMCHAR 28

struct Num 
{
	int leftOE;
	int rightOE;
};

struct Modul
{
	int poz;
	int dest;
};

extern CString Valide;
extern Modul m_moduli[101];

	//search the character c within the string str. if str=valide, it searches whether the character is number, 
	//operator, etc.

inline int SearchGroup(const CString& str, WCHAR c)
{
	for (int i=0; i<str.GetLength(); i++)
		if (str[i]==c) 
		{
			if (i>=0 && i<=5) return SHGROUP_OPERATOR;//for valide: operators
			if (i>=6 && i<=14) return SHGROUP_PARANTHESIS;//for valide: paranthesis
			if (i==15) return SHGROUP_COMMA;//for valide: comma (,)
			if (i==16) return SHGROUP_EQUAL;//for valide: equal
			if (i>=17 && i<=27) return SHGROUP_NUMBER;//for valide: number (including ".")
			if (i==28) return SHGROUP_SPACE; //space (" ")
		}
	return SHGROUP_NOGROUP;//unknowngroup
}

	//searches the character in the string, from the position From until To and returns the position where 
	//it was found; -1 if not found
//	int Search(const CString&, char, int From = 0, int To = MAXIMCHAR);//26=MAXCHAR
inline int Search(const CString& str, WCHAR c, int From = 0, int To = MAXIMCHAR)
{
	if (From>To) return -1;
	for (int i=From; i<=To; i++)
		if (str[i]==c) return i;
	return -1;
}
	//checks to see if str is a constant/function/modulo or 0 if other
//	int FoundFunction(const CString& str);
inline int FoundFunction(const CString& str)
{
	if (!str.CompareNoCase(L"E")) return 1;
	else
	if (!str.CompareNoCase(L"PI")) return 2;
	else

	if (!str.CompareNoCase(L"RAD")) return 10;
	else
	if (!str.CompareNoCase(L"LOG")) return 11;
	else
	if (!str.CompareNoCase(L"LN")) return 12;
	else
	if (!str.CompareNoCase(L"LG")) return 13;
	else

	if (!str.CompareNoCase(L"CMMDC")) return 20;
	else
	if (!str.CompareNoCase(L"CMMMC")) return 21;
	else

	if (!str.CompareNoCase(L"MIN")) return 30;
	else
	if (!str.CompareNoCase(L"MAX")) return 31;
	else

	if (!str.CompareNoCase(L"P")) return 40;
	else
	if (!str.CompareNoCase(L"A")) return 41;
	else
	if (!str.CompareNoCase(L"C")) return 42;
	else

	if (!str.CompareNoCase(L"MOD")) return 50;
	else  return 0;
}
	//gets the substring from the string, between positions x and y
//	CString GetFunction(const CString& m_strExpression, int x, int y);
inline CString GetFunction(const CString& m_strExpression, int x, int y)
{
	CString str;

	if (x<0 || y>m_strExpression.GetLength()-1) return _T("");
	for (int i=x; i<=y; i++) str+=m_strExpression[i];

	return str;
}
	//gets the number of module paranthesis and finds the position of each.
//	int GetNrM(const CString& m_strExpression, int From, int To);
inline int GetNrM(const CString& m_strExpression, int From, int To)
{
	int nrM = 0;
	for (int i=From; i<=To; i++) if (m_strExpression[i]=='|')
	{
		nrM++;
		m_moduli[nrM].poz = i;
	}

	return nrM;
}
	//gets the name NAME_ of the element starting at the position from in the string, and returns its margins
//	int GetName(const CString& m_strExpression, int From, int& left, int& right);
inline int GetName(const CString& m_strExpression, int From, int &left, int &right)
{
	if (From<0 || From>m_strExpression.GetLength()-1)
	{
		left = -1;
		right = -1;
		return NAME_ERROR;//0;
	}

	int x = SearchGroup(Valide, m_strExpression[From]);
	int y = Search(Valide, m_strExpression[From]);

	//Numar
	if (x==SHGROUP_NUMBER) 
	{
		left = From;
		while (x==SHGROUP_NUMBER)
		{
			From++;
			x = SearchGroup(Valide, m_strExpression[From]);
		}
		right = --From;
		return NAME_NUMBER;//1;
	}

	//Functie
	if (x==SHGROUP_NOGROUP) 
	{
		left = From;
		while (x==SHGROUP_NOGROUP)
		{
			From++;
			x = SearchGroup(Valide, m_strExpression[From]);
		}
		right = --From;
		return NAME_FUNCTION;//3;
	}

	left = right = From;

	if (x==SHGROUP_OPERATOR) return NAME_OPERATOR;//4;
	if (x==SHGROUP_PARANTHESIS) return NAME_PARANTHESIS;//2;
	if (x==SHGROUP_EQUAL)  return NAME_EQUAL;//5;

	return NAME_ERROR;
}
	//gets the CHARTYPE_ of the char
//	int GetCharType(char c);
inline int GetCharType(WCHAR c) 
{

	if (c=='+' || c=='-') return CHARTYPE_ADDITION;//1;
	if (c=='*' || c=='/' || c=='^') return CHARTYPE_MULTIPLICATION;//2;
	if (c=='!') return CHARTYPE_FACTORIAL;//3;
	if (c=='(' || c=='[' || c=='{' || c=='<' || c=='|') return CHARTYPE_OPENEDPARANTHESIS;//4;
	if (c==')' || c==']' || c=='}' || c=='>') return CHARTYPE_CLOSEDPARANTHESIS;//5;
	if (c==',') return CHARTYPE_COMMA;//7;
	if (c=='=') return CHARTYPE_EQUAL;//8;
	if (c==' ') return CHARTYPE_SPACE;//10;

	if (SearchGroup(Valide, c)==SHGROUP_NUMBER) return CHARTYPE_NUMBER;//9;

	return CHARTYPE_OTHER;//0;
}
	//checks if the expression that begins from the character From, within the string, is a function
//	BOOL IsFunction(const CString& m_strExpression, int From);
inline BOOL IsFunction(const CString& m_strExpression, int From)
{
	int left, right, x;
	GetName(m_strExpression, From, left, right);
	x = FoundFunction(GetFunction(m_strExpression, left, right));
	
	if (x>9 && x<50) return TRUE;
	return FALSE;
}
	//checks if the character starting from the position From is an operator
//	BOOL IsOperator(const CString& m_strExpression, int From);
inline BOOL IsOperator(const CString& m_strExpression, int From)
{
	if (SearchGroup(Valide, m_strExpression[From])==1)
		return TRUE;

	int left, right;
	GetName(m_strExpression, From, left, right);
	if (FoundFunction(GetFunction(m_strExpression, left, right))==50) return TRUE;
	return FALSE;
}
	//
//	int GetBkName(const CString& m_strExpression, int End, int &left, int &right);
inline int GetBkName(const CString& m_strExpression, int End, int &left, int &right)
{

	if (End<0 || End>m_strExpression.GetLength()-1)
	{
		left = -1;
		right = -1;
		return NAME_ERROR;
	}

	int x = SearchGroup(Valide, m_strExpression[End]);
	int y = Search(Valide, m_strExpression[End]);

	//Numar
	if (x==SHGROUP_NUMBER) 
	{
		right = End;
		while (x==SHGROUP_NUMBER && End>=0)
		{
			End--;
			if (End>=0) x = SearchGroup(Valide, m_strExpression[End]);
		}
		left = ++End;
		return NAME_NUMBER;//1
	}

	//Functie
	if (x==SHGROUP_NOGROUP) 
	{
		right = End;
		while (x==SHGROUP_NOGROUP && End>=0)
		{
			End--;
			if (End>=0) x = SearchGroup(Valide, m_strExpression[End]);
		}
		left = ++End;
		return NAME_FUNCTION;//3;
	}

	left = right = End;

	if (x==SHGROUP_OPERATOR) return NAME_OPERATOR;//4;
	if (x==SHGROUP_PARANTHESIS) return NAME_PARANTHESIS;//2;
	if (x==SHGROUP_EQUAL) return NAME_EQUAL;//5;

	return NAME_ERROR;//0;
}

inline int GetNrNrs(CString& m_strExpression, int From, int To)
{
	int nr(0), left, right;

	if (From<0 || From>m_strExpression.GetLength()-1 || From>To)
		return 0;

	while (From<=To)
	{
		int Name = GetName(m_strExpression, From, left, right);
		int func = FoundFunction(GetFunction(m_strExpression, left, right));

		if (Name ==1 || Name ==3 && func<9 && func>0)
		{
			nr++;
			From= right+1;
		}

		else From++;
	}

	return nr;
}


inline BOOL GetNextNumber(const CString& m_strExpression, int From, BOOL Forward, double &nr)
{
	if (From<0 || From>m_strExpression.GetLength()-1)
	{
		nr = 0;
		return FALSE;
	}

	int x, left, right, poz(From);
	BOOL neg(FALSE);

	if (Forward)	//* 543
	{
		if (m_strExpression[From]==' ') poz++;
		if (GetCharType(m_strExpression[From])==CHARTYPE_OPENEDPARANTHESIS)	//lg[-10]=...
		{
			poz++;
			if (m_strExpression[From+1]=='+') poz++;
			else if (m_strExpression[From+1]=='-')
			{
				poz++;
				neg = TRUE;
			}
		}

		else
		if ((m_strExpression[poz]=='-' || m_strExpression[poz]=='+') /*&& 
			(poz==0 || GetCharType(m_strExpression[poz-1])==4 || m_strExpression[poz-1]==','))*/)
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			if (m_strExpression[poz]=='-') neg = TRUE;
			poz++;
		}
		x = GetName(m_strExpression, poz, left, right);
		if (x==1) nr =_wtof(GetFunction(m_strExpression, left, right));
		else if (x==3)
		{
			x = FoundFunction(GetFunction(m_strExpression, left, right));
			if (x==1) nr = M_E;
			else if (x==2) nr = M_PI;
			else return FALSE;
		}

		else return FALSE;
	}

	else 
	{
		if (m_strExpression[From]==' ') poz--;
		if (m_strExpression[From]==')') poz--;

		x = GetBkName(m_strExpression, poz, left, right);
		if (x==1) nr = _wtof(GetFunction(m_strExpression, left, right));
		else if (x==3)
		{
			x = FoundFunction(GetFunction(m_strExpression, left, right));
			if (x==1) nr = M_E;
			else if (x==2) nr = M_PI;
			else return FALSE;
		}

		else return FALSE;
		if (left-1>=0 && m_strExpression[left-1]=='-' && (left-2>=0 && m_strExpression[left-2]=='(' || left-1==0)) neg = TRUE;
	}

	if (neg) nr = -nr;

	return TRUE;

}

#endif