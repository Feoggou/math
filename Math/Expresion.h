// Expresion.h: interface for the CExpresion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPRESION_H__4DF6BD6D_B48D_469E_827B_CAAFF4A4919C__INCLUDED_)
#define AFX_EXPRESION_H__4DF6BD6D_B48D_469E_827B_CAAFF4A4919C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Validation.h"

class CExpresion  
{
public:
	CString ReturnString();
	//places the string str as the object's main string.
	void GetString(CString str);
	//calculates the expression
	void Calculate();
	BOOL CheckValid();
	int GetHighBeg();
	int GetHighEnd();
	int GetErrorPos1();
	int GetErrorPos2();


	CExpresion();
	virtual ~CExpresion();

protected:

	BOOL IsSingle(int From, int To);
	void PrepareString();
	BOOL IsNumberParan(int poz, BOOL Forward);
	int GetBegOfExpr(int nEnd);
	double modulo(double a, double b);
	CString CalcC(double a, double b);
	CString CalcA(double a, double b);
	CString CalcP(double n);
	CString CalcMax(double a, double b);
	CString CalcMin(double a, double b);
	CString CalcCMMMC(double a, double b);
	CString CalcCMMDC(double a, double b);
	CString CalcLog(double a, double b);
	CString CalcRad(double a, double b);
	CString CalcFunction(CString func, int &From, int& left, int& right);
//	BOOL GetNextNumber(int From, BOOL Forward, double &nr);
	double perm(double n);
	CString CalcOperation(CString op, int left, int right);
	int GetBackOp(CString &op, int From, int To);
	int GetOperatorValue(int From, int To);
	int GetNext(int From, BOOL Forward, int &Name);
	CString GetDigits(double nr, BOOL eFractie = FALSE);
	CString dtoa(double nr, CString &result);
	CString CalcParan(int From, int To);
	void AddToString(CString result, int left, int right, BOOL ReplaceParan = FALSE);
	BOOL IsMostImportant(int poz, int &left, int &right);
	BOOL VerifyLeftRight(int poz);
	int GetNextOp(CString &op, int From, int To);
	void Calc(int From, int To);

	//Functii

protected:
	//Variabile
	CString m_strExpression;
	CValidation m_Validation;

	int Contor;
	CString t;
	int nrM;
};

#endif // !defined(AFX_EXPRESION_H__4DF6BD6D_B48D_469E_827B_CAAFF4A4919C__INCLUDED_)
