#pragma once

#include "expDefines.h"

class CValidation
{
public:
	CValidation(CString& m_strExpression);
	~CValidation(void);

	//public variables:
public:
	BOOL m_bStop;
	int nrVarPos;
	int HighBeg, HighEnd;
	int err_pos1, err_pos2;

public:
//	void SetExpression(CString str);
	//checks for validation
	BOOL Validate(CString& m_strExpression);
	//searches to see if "=" exists
	BOOL EqualSignExists();
	//triggers an error, to know wether to stop or not the validation/calculation process.
	void SendError(int cod, int From=-1, int To=-1);
	//gets the closed paranthesis coresponding the opened paranthesis; returns error if the case
	int GetEndOfExpr(int nFirst);
	//???
	int SearchVPos(int From, int To);
protected:
	//protected functions
	//Eliminates the spaces from the string
	void EliminateSpaces();
	//checks to see if the elements within the string are valid (exist in program's knowledge).
	BOOL ElementsExist();
	//checks to see if the elements are good arranged (not like "5+/4rad*6=")
	BOOL GoodArranged(int From, int To);
	//check the validity of paranthesis
	int ValidateParanthesis(int& left, int& right);
	//transforms modules from | | into < > and checks their validity: returns true if valid.
	BOOL ArrangeModules();
	//checks the validity of the number(in string form):
	int ValidateNumber(int left, int right);
	//tests in the right part
	void TestRight(int Poz, WCHAR c, int To);
	//for ArrangeModules
	BOOL Continue(int);
	//
	BOOL TestExpression(int x, int y, int c);
	//
	BOOL GetParanArr(int From, int To);

protected:
	//protected variables:
	CString& m_sMain;
};
