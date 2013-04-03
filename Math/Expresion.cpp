// Expresion.cpp: implementation of the CExpresion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "expDefines.h"
#include "Expresion.h"

#include <math.h>
#include "expUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CString CExpresion::Valide;
extern int nrMaxim;
extern bool ShowResult;
extern CString Valide;

int LastOperatorExecuted = -1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExpresion::CExpresion()
:m_strExpression(_T("")),
m_Validation(m_strExpression)
{
	Contor = 0;
}

CExpresion::~CExpresion()
{

}

void CExpresion::GetString(CString str)
{
	m_strExpression = str;
}

CString CExpresion::ReturnString()
{
	return m_strExpression;
}

void CExpresion::Calc(int From, int To)
{

	//verifica pt 4!, -4!, etc...


	m_Validation.nrVarPos = 0;		//folosesc variabila asta k daca am trecut prin egal sau nu
	int left(0), right(0);
	int Current(From), poz(0);
	CString op;

	while (Current<=To && !m_Validation.m_bStop)
	{
		poz = GetNextOp(op, Current, To);
		if (poz==-1) { Current=To+1; continue;}
		GetName(m_strExpression, poz, left, right);

		//finalul liniei
		if (op[0]=='=')
		{
			double nr;
			int x;
			if (m_strExpression[0]=='+' || m_strExpression[0]=='-') x = FoundFunction(GetFunction(m_strExpression, 1, poz-1));
			else x = FoundFunction(GetFunction(m_strExpression, 0, poz-1));
			if (GetNrNrs(m_strExpression, 0, poz-1)==1 && x>0 && x<9)
			{
				GetNextNumber(m_strExpression, poz-1, FALSE, nr);
				dtoa(nr, t);
				return;
			}
			t+= GetFunction(m_strExpression, Contor, poz-1);
			t+='=';
			right = poz;
			m_Validation.nrVarPos = 1;
			LastOperatorExecuted = -1;
		}

		//Subexpresie = incadrare
		else if (GetCharType(op[0])==CHARTYPE_OPENEDPARANTHESIS)
		{
			int x = m_Validation.GetEndOfExpr(poz);

			if (GetNrNrs(m_strExpression, poz+1, x-1)>1 ||!IsSingle(poz+1, x-1))
			{
				Calc(poz+1, x-1);
				right = x;
			}

			//nrnrs == 1
			else
			{
				CString result = CalcParan(poz, x);

				//[], {}, <>
				if (result.GetLength()) 
				{
					AddToString(result, poz, x, TRUE);
					right = x;
				}
			}
		}

		//operator
		else if (IsOperator(m_strExpression, poz))
		{
			if (VerifyLeftRight(poz))
			{
				if (IsMostImportant(poz, left, right))
				{
					CString result;
					result = CalcOperation(op, left, right);
					AddToString(result, left, right, FALSE);
				}
			}
		}

		//functie
		else if (IsFunction(m_strExpression, poz))
		{
			CString result;
			result = CalcFunction(op, poz, left, right);
			if (result.GetLength()) AddToString(result, left, right, TRUE);
		}

		Current = right+1;
	}

/*	if (!nrVarPos && To<s.GetLength()-1 && GetCharType(s[To+1])!=5)
	{
		t+=GetFunction(m_strExpression, Contor, To);
//		if (Contor<=To) Contor = To+1;
	}*/
}

int CExpresion::GetNextOp(CString &op, int From, int To)
{
	if (m_Validation.m_bStop) return -1;

	if (From<0 || To>m_strExpression.GetLength()-1 || From>To)
	{
		op = _T("");
		return -1;
	}

	int left, right, x;

	while (From<=To)
	{
		if (m_strExpression[From]==',') { op=_T(""); return From;}
		op = _T("");
		x = SearchGroup(Valide, m_strExpression[From]);
		if (x==6) x = 0;
		if (m_strExpression[From]=='(')
		{
			int i = m_Validation.GetEndOfExpr(From);
			if (GetNrNrs(m_strExpression, From, i)==1) From = i;
			else {op = "("; return From;}
		}

		else if (x)
		{
			if (x!=5)
			{
				op+=m_strExpression[From];
				return From;
			}
		}

		else {
			if (m_strExpression[From]==' ') From++;
			GetName(m_strExpression, From, left, right);
			if (FoundFunction(op = GetFunction(m_strExpression, left, right))>9)
				return From;
			else From = right;
		}

		From++;
	}

	return -1;
}

BOOL CExpresion::VerifyLeftRight(int poz)
{
	int x, left, right, name, nr, ct, Corect(0);
	//Corect: 0 - nu e corect, 1 - corect unu, 2 - corect amandoua

	//center
	GetName(m_strExpression, poz, left, right);
	nr = FoundFunction(GetFunction(m_strExpression, left, right));
	if (nr>9 && nr<50) return FALSE;//function, not constant, not module

	//right
	if (m_strExpression[poz]=='!') Corect++;
	else {
		nr = right+1;
		x = GetNext(nr, TRUE, name);
		int l, r;
		GetBkName(m_strExpression, x, l, r);
		ct = FoundFunction(GetFunction(m_strExpression, l, r));
		if (name==1 || name==3 && ct<9 && ct>0) Corect++;
	}

	//left
	nr = poz-1;
	x = GetNext(nr, FALSE, name);
	if (x>=0)
	{
		int l, r;
		GetName(m_strExpression, x, l, r);
		ct = FoundFunction(GetFunction(m_strExpression, x, nr));
		if (name==1 || name==3 && ct<9 && ct<9) Corect++;
	}

	if (Corect==2) return TRUE;
	else return FALSE;
}

BOOL CExpresion::IsMostImportant(int poz, int &left, int &right)
{

	int xL, x, xR, i, l, r, nr, Verify(0);
	CString op;

	//Verify: 0 - nu e paran, 1 - paran st, 2 - paran dr

	//center
	GetName(m_strExpression, poz, l, r);
	x = GetOperatorValue(l, r);

	//right
	nr = r+1;
	i = GetNextOp(op, nr, m_strExpression.GetLength()-1);
	GetName(m_strExpression, i, l, r);
	if (l>-1 && op.GetLength()) xR = GetOperatorValue(l, r);
	else xR=0;
	right = l-1;
	
	//left
	nr = poz-1;
	i = GetBackOp(op, 0, nr);
	GetBkName(m_strExpression, i, l, r);
	if (l>-1 && i!=0 && (l==0 || GetCharType(m_strExpression[l-1])!=4) || l>-1 && IsFunction(m_strExpression, l))
	{
		if (m_strExpression[l]=='!') xL = 0;
		else xL = GetOperatorValue(l, r);
	} else xL = 0;
/*	if ((l>0 && GetCharType(s[l-1])!=4 || l==-1) ||
		(l==0 && i!=0)) left = r+1;
	else left = r;*/
	//commentary:
	//if the operator is first (or is not but the preceding is '(' or ',') and is char type is addition(+/-) 
	//and the main operator (the checked one, the one in right) is not !, or if it is, it is the paranthesis to 
	//factorial, ONLY THEN we add that operator as unary operator of that number.
/*	if ((l==0 || l>0 && (GetCharType(m_strExpression[l-1])==4 || m_strExpression[l-1]==',')) && GetCharType(m_strExpression[l])==1 &&
		(m_strExpression[poz]!='!' || m_strExpression[poz]=='!' && m_strExpression[poz-1]==')')) left = r;
	else
	left = r+1;*/

	//but what if we have -9-9-9-9??? that way we will have -9-9-(9-9) instead of -9-9+(-9-9)
	//so we try:
	if (l > -1 && GetCharType(m_strExpression[l]) == CHARTYPE_ADDITION &&
		(m_strExpression[poz]!='!' || m_strExpression[poz]=='!' && m_strExpression[poz-1]==')')) left = r;
	else
	left = r+1;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//return value
	//pt x>xL mio dat = knd treia sa calc un "log(..,-5+3=" si treia sa treaca true
	// dar pt 5*1.29/10-9 = 6.45/10-9 imi trece 6.450.12-9 CEEA CE E RAU!
	//dar pt paranteza de tipul (1+6.45/10-9)*2*2= nu imi calculeaza 2*2 desi se poate calcula!
	if (x>xL && x>=xR) 
	{
		LastOperatorExecuted = poz;
		return TRUE;
	}
	else if (x>=xL && LastOperatorExecuted != l/*left of operator op*/ && x>=xR)
	{
		LastOperatorExecuted = poz;
		return TRUE;
	}
	else return FALSE;
}

void CExpresion::Calculate()
{
//	MessageBox(NULL, "IsValid->TRUE", "verificare", MB_OK);
	CString result = L"\t";

	m_Validation.HighBeg = m_Validation.HighEnd = -1;
	m_Validation.err_pos1 = m_Validation.err_pos2 = -1;

	result+=m_strExpression;
	PrepareString();
	while (m_Validation.EqualSignExists() && !m_Validation.m_bStop)
	{//37
		for (int i=0; i<m_strExpression.GetLength(); i++)
		{
			int iTo(0), left(0), right(0);
			if (m_strExpression[i]=='(') {iTo= m_Validation.GetEndOfExpr(i);GetBkName(m_strExpression, i-1, left, right);}
			if (m_strExpression[i]=='(' && GetNrNrs(m_strExpression, i+1, iTo-1)==1 && IsSingle(i+1, iTo-1) && 
				GetCharType(m_strExpression[i+1])!=1 && !IsFunction(m_strExpression, left))
			{
				m_strExpression.Delete(i);
				m_strExpression.Delete(iTo);
				i--;
			}
		}
		Contor = 0;
		Calc(0, m_strExpression.GetLength()-1);
		CString aux = m_strExpression;
		m_strExpression=t;
		if (m_strExpression[m_strExpression.GetLength()-1]=='=' &&
			GetNrNrs(m_strExpression, 0, m_strExpression.GetLength()-1)==1 &&
			IsSingle(0, m_strExpression.GetLength()-2)) m_strExpression.Delete(m_strExpression.GetLength()-1);

//		result+="\r\n\t";
		result+="\n\t";
		result+=m_strExpression;
//		MessageBox(NULL, result, "am reusit...", MB_OK);

		t = _T("");
		if (m_strExpression.CompareNoCase(aux)==0)
		{
			MessageBox(0, L"A fost detectatã repetiþie infinitã. Contactaþi dezvoltatorul aplicaþiei!", L"Error!", MB_ICONERROR);
			return;
		}
	}

	// Format: <5+3> -> |5+3|
	for (int i=0; i<result.GetLength(); i++)
		if (result[i]=='<' || result[i]=='>')
		{
			result.Delete(i);
			result.Insert(i,L"|");
		}


	if (!ShowResult) m_strExpression = result;
}

void CExpresion::AddToString(CString result, int left, int right, BOOL ReplaceParan)
{
	if (m_Validation.m_bStop) return;

	CString nou, Error;

	Error = result.Left(5);
	if (!Error.Compare(L"NULL "))
	{
//		t+= "\r\n\r\n";
		t+= "\n\n";
		result.Replace(result, result.Right(result.GetLength()-5));

		t+= result;
		
		m_Validation.m_bStop = TRUE;
		return;
	}

	if (left-1<0)
	{
		t+= result;
		Contor = right+1;
		return;
	}

	int nrL, nrR;

	//if encloded in paranthesis
	if (GetCharType(m_strExpression[left-1])==CHARTYPE_OPENEDPARANTHESIS && 
		GetCharType(m_strExpression[right+1])==CHARTYPE_CLOSEDPARANTHESIS)
	{
		nrL = left-1;
		nrR = right+1;
	}
	//if we start with a paranthesis
	if (GetCharType(m_strExpression[left])==CHARTYPE_OPENEDPARANTHESIS)
	{
		nrL = left;
		nrR = right;
	}

	//if enclosed in paranthesis or begins with a paranthesis
	if (GetCharType(m_strExpression[left-1])==CHARTYPE_OPENEDPARANTHESIS && 
		GetCharType(m_strExpression[right+1])==CHARTYPE_CLOSEDPARANTHESIS ||
		GetCharType(m_strExpression[left])==CHARTYPE_OPENEDPARANTHESIS) // (a...) sau (...)
	{
		if (result[0]=='-' && m_strExpression[nrL]=='(')         //(-a...)
		{
			if (nrL > 0 && GetCharType(m_strExpression[nrL-1])==4)	//((-a...)
			{
				nou = GetFunction(m_strExpression, Contor, nrL-1);
				t+= nou;
				t+= result;
				Contor = nrR+1;
			}

			else if (nrL > 0 && m_strExpression[nrL-1]=='+')	//+(-a...)
			{
				nou = GetFunction(m_strExpression, Contor, nrL-2);
				t+= nou;
				Contor = nrR+1;
				t+= result;
			}

			else if (nrL > 0 && m_strExpression[nrL-1]=='-')	//-(-a...)
			{
				nou = GetFunction(m_strExpression, Contor, nrL-2);
				t+=nou;
				result+= '+';
				Contor = nrR + 1;
				t+= result;
			}

			else {						//*(-a...)
				nou = GetFunction(m_strExpression, Contor, nrL);
				if (GetCharType(m_strExpression[nrL])!=4) nou+=m_strExpression[nrL];
				t+= nou;
				t+= result;
				t+= m_strExpression[m_Validation.GetEndOfExpr(nrL)];
				Contor = nrR + 1;
			}
		}

		//! (-...) pt paranteze, fara rezultat negativ

		else {
			nou = GetFunction(m_strExpression, Contor, nrL);
			int x = GetNrNrs(m_strExpression, left, right);
			int st, dr;
			GetBkName(m_strExpression, nrL-1, st, dr);

			if (GetCharType(m_strExpression[nrL])==4 && (!IsFunction(m_strExpression, st)) &&
				(x==1 || x==2 && (m_strExpression[nrL]=='(' || m_strExpression[nrL]=='<' && result[0]!='-'))) /* && ReplaceParan)*/
			{
				nou.Delete(nou.GetLength()-1);
//				nou+= '(';
			}
			t+= nou;
			t+= result;

			if (GetCharType(m_strExpression[nrR])==5 && (!IsFunction(m_strExpression, st)) &&
				(x==1 || x==2 && (m_strExpression[nrR]==')' || m_strExpression[nrR]=='>' && result[0]!='-')))/* && ReplaceParan)*/
			{
//				t.Delete(nrR);
//				t+= ')';
				Contor = nrR+1;
			}
			else Contor = nrR;
		}
	}

	//! (...)

	else {

//		if (left-1==0 && GetCharType(s[left-1])==1) 
		//!!!!!!!!!!!!!!!!!!!!!!!!!!! si aici e problema q +,- care nu sunt neaparat dupa paranteza/virgula:
		//-18-18+9-1.25-2*1.41= da -367.25-2*1.41=
		
		nou = GetFunction(m_strExpression, Contor, left-1);
		int aleft, aright;
		
		if (nou.GetLength())
		{
			if (nou[nou.GetLength()-1]=='+')
			{
				if (result[0]=='-') nou.Delete(nou.GetLength()-1);
			}
			else if (nou[nou.GetLength()-1]=='-')//nou = '-';
			{
				if (result[0]=='-') {result.Delete(0); nou.Delete(nou.GetLength()-1); nou+= '+';}
			}
			t+= nou;
		}
		//added 2009
		//if we were left ended with "+6" or "6!" and we have to add "7" (which got to be a positive number) we MUST 
		//add +!
		int bkName = GetBkName(t, t.GetLength()-1, aleft, aright);
		if ((bkName == NAME_NUMBER || t.Right(1)==L"!") && result[0]!='-' && result[0]!='+')
		{
			//so the last character of t is number, and result has no sign
			t += '+';
		}
		//if we ended up with an operator (but "!") and we add something like "+5" or "-7" we must take care!
		if (bkName == NAME_OPERATOR && t.Right(1)!= L"!")
		{
			if (result[0]=='+') result.Delete(result.GetLength()-1);
			else if (result[0]=='-')
			{
				t+='(';
				result+=')';
			}
		}

		t+= result;
		Contor = right+1;
	}
}

inline CString CExpresion::CalcParan(int From, int To)
{
	CString result;

	double nr;

	if (GetNrNrs(m_strExpression, From+1, To-1)>1) return _T("");
	GetNextNumber(m_strExpression, From, TRUE, nr);

	switch(m_strExpression[From])
	{
	case '[': nr = floor(nr); break;
	case '{': nr = nr - floor(nr); break;
	case '<': nr = nr>0 ? nr : -nr; break;
	}

	dtoa(nr, result);
	return result;
}

inline CString CExpresion::dtoa(double nr, CString &result)
{
	//validation
	if (nr>9999999999999)  return L"NULL Eroare! Rezultatul este format din prea multe cifre!";

	result = L"";
	result.Format(L"%.15f", nr);
	//modifying a bit the string: if 5/2 = 2.49999999998 we add values
/*	int the_pos = -1;
	BOOL unu_in_minte = FALSE;
	if (result.Right(1) == L"8" || result.Right(1) == L"9")
	{
		result.SetAt(result.GetLength()-1, '0');
		unu_in_minte = TRUE;
		the_pos = result.GetLength()-2;//-1 = last; 
	}

	for (int i = the_pos; i>=0; i--)
	{
		if (result[i] == '.') continue;

		if (result[i]!='9') 
		{
			//if things were added (this happens if we passed through this function)
			//this is the only place in which we add one more and exit... but what digit is it?
			switch (result[i])
			{
			case '0': result.SetAt(i, '1'); break;
			case '1': result.SetAt(i, '2'); break;
			case '2': result.SetAt(i, '3'); break;
			case '3': result.SetAt(i, '4'); break;
			case '4': result.SetAt(i, '5'); break;
			case '5': result.SetAt(i, '6'); break;
			case '6': result.SetAt(i, '7'); break;
			case '7': result.SetAt(i, '8'); break;
			case '8': result.SetAt(i, '9'); break;
			}
			unu_in_minte = FALSE;
			break;
		}
		//if the digit is 9, we add one, making it 10. i takes the place of the_pos

		result.SetAt(i, '0');
	}

	//we ended adding... but what if we originally had 9.99999? it should not turn 0.00000!
	//this can happen only if we still have unu_in_minte == TRUE
	if (unu_in_minte) result.Insert(0, '1');*/

	//checking the fractional part.
	//we first remove any 0 from the end, if points to '.'
	int i = result.Find('.');
	if (i > -1)
	{
		int j = result.GetLength()-1;
		//we also remove '.' if there are 0-s after
		while ((result[j]=='0' || result[j]=='.') && j>=i)
		{
			result.Delete(j);
			j--;
		}
	}

	i = result.Find('.');
	if (i > -1)
	{
		//now we must have the value in specific number of digits!
		int nrDecimals = 0;
		while (i > -1 && nrDecimals <= nrMaxim && i < result.GetLength())
		{
			//if (isdigit(result[i])) 
			nrDecimals++;
			i++;
		}
		result = result.Left(i);
	}

	return result;
}

inline int CExpresion::GetNext(int From, BOOL Forward, int& Name)
{

	if (From<0 || From>m_strExpression.GetLength()-1)
	{
		Name = 0;
		return -1;
	}

	int left, right;

	if (Forward)
	{
		if (m_strExpression[From]==' ') From++;
		if (m_strExpression[From]=='(' && GetNrNrs(m_strExpression, From, m_Validation.GetEndOfExpr(From))==1)
		{
			From++;
			if (GetCharType(m_strExpression[From])==1) From++;
		}

		Name = GetName(m_strExpression, From, left, right);
		return right;
	}

	else {
		if (m_strExpression[From]==' ') From--;
		if (m_strExpression[From]==')' && GetNrNrs(m_strExpression, GetBegOfExpr(From), From)==1) From--;
		Name = GetBkName(m_strExpression, From, left, right);
		return left;
	}
}

inline int CExpresion::GetOperatorValue(int From, int To)
{

	if (From<0 || From>m_strExpression.GetLength()-1 || From>To)
		return 0;

	int x(0);
	
	if (From==To)
	{
		x = Search(Valide, m_strExpression[From]);
		if (x==-1 && FoundFunction(GetFunction(m_strExpression, From,To))==40) return 5;
		if (x>=0 && x<=1) return 1;
		else if (x>1 && x<=3) return 2;
		else if (x==4) return 3;
		else if (x==5) return 6;
//		else if (x!=16) return 10;
		else return 0;
	}
	else {
		x=FoundFunction(GetFunction(m_strExpression, From, To));
		if (x==50) return 2;
		else if (x>9 && x<55) return 5;
//		else if (x!=16) return 10;
		else return 0;
	}
}

inline int CExpresion::GetBackOp(CString &op, int From, int To)
{

	if (From<0 || To>m_strExpression.GetLength()-1 || From>To)
	{
		op = _T("");
		return -1;
	}
	while (To>=From)
	{
		op = _T("");
		int left(0), right(0);
		if (m_strExpression[To]==')')
		{
			int i = GetBegOfExpr(To);
			if (GetNrNrs(m_strExpression, i, To)==1) To = i-1;
			if (To<0) return -1;
		}

		int x = SearchGroup(Valide, m_strExpression[To]);
		if (x==6) x = 0;
		if (x)
		{
			if (x!= 5)
			{
				op+= m_strExpression[To];
				return To;
			}
		}

		else {
			if (m_strExpression[To]==' ') To--;
			GetBkName(m_strExpression, To, left, right);
			if (FoundFunction(op = GetFunction(m_strExpression, left, right))>9) return To;
			else To = left;
		}
		To--;
	}

	return -1;
}

CString CExpresion::CalcOperation(CString op, int left, int right)
{

	CString result;

	if (op!= "!")
	{
		double a, b;
		if (GetNextNumber(m_strExpression, left, TRUE, a) && GetNextNumber(m_strExpression, right, FALSE, b))
		{
			if (op[0]=='+') dtoa(a+b, result);
			else if (op[0]=='-') dtoa(a-b, result);
			else if (op[0]=='*') dtoa(a*b, result);
			else if (op=='/')
			{
				if (b==0) result = "NULL EROARE! Al doilea operand trebuie sã nu fie nul!";
				else dtoa(a/b, result);
			}

			else if (op[0]=='^')
			{
				if (a==0 && b==0) result = "NULL EROARE! Operanzii nu au voie sã fie ambii nuli!";
				else 
				{
					//we may have (-2)^(1/2) which is impossible
					if (a < 0)
					{
/*						if (b < 1 && b > -1) result = "NULL EROARE! Un numar negativ nu se poate ridica la putere\
subunitara!";*/
						if (b != floor(b)) result = "NULL EROARE! Un numar negativ nu se poate ridica la putere\
rationala/reala!";
						else 
						{
							double fResult = pow(abs(a),b);
							fResult = -fResult;
							dtoa(fResult, result);
						}
					}
					else//a>=0 => calculate normal!
						dtoa(pow(a, b), result);
				}
			}

			else if (!(op.CompareNoCase(L"mod")))
			{
				if (a!= floor(a)) result = "NULL EROARE! Primul operand trebuie sã fie întreg!";
				if (b!= floor(b)) result = "NULL EROARE! Al doilea operand trebuie sã fie întreg!";
				if (b!= floor(b) || b<=0) result = "NULL EROARE! Al doilea operand trebuie sã fie natural nenul!";

				if (!result.GetLength()) dtoa(modulo(a,b), result);
			}
		}
	}
	else {
		double a;

		if (GetNextNumber(m_strExpression, right-1, FALSE, a))
		{
//			dtoa(perm(a), result);
			if (a<0 && m_strExpression[right-1]!=')') a= -a;
			result = CalcP(a);
		}
	}

	return result;
}

inline double CExpresion::perm(double n)
{
	double nr(1);

	if (n==0) return 1;
	for (int i=1; i<=n; i++)
		nr*=i;

	return nr;
}


CString CExpresion::CalcFunction(CString func, int &From, int& left, int& right)
{

	int first(0), last(0);
	CString result;
	BOOL goFunc(TRUE);
	double a,b;

	GetName(m_strExpression, From, left, right);
	if (GetCharType(m_strExpression[right+1])==4)
	{
		first = right+1;
		last = m_Validation.GetEndOfExpr(right+1);
	}

	if (first!= last)	//(...)
	{
//		int x = Search(s,',', first, last-1);

		int x = m_Validation.SearchVPos(first, last-1);
		if (x>-1)	//(... , ...)
		{
			if (!(GetNrNrs(m_strExpression, first+1, x-1)==1 && IsSingle(first+1, x-1)) || 
				!(GetNrNrs(m_strExpression, x+1, last-1)==1 && IsSingle(x+1, last-1)))
			{
				goFunc = FALSE;
				if (!(GetNrNrs(m_strExpression, first+1, x-1)==1 && IsSingle(first+1, x-1)))
				{
					Calc(first+1, x-1);
					t+=GetFunction(m_strExpression, Contor, x-1);
					t+=',';
					if (Contor<=x-1) Contor=x+1;
					else Contor++;
				}
/*				right = last;
			}

			if (GetNrNrs(m_strExpression, x+1, last-1)>1)
			{*/
				goFunc = FALSE;
				Calc(x+1, last-1);
//				t+=s[last];
				right = last;
			}

			if (!goFunc) { From = last; return _T("");}
			GetNextNumber(m_strExpression, first+1, TRUE, a);
			GetNextNumber(m_strExpression, x+1, TRUE, b);

			switch (FoundFunction(func))
			{
			case 10: result = CalcRad(a,b); break;
			case 11: result = CalcLog(a,b); break;
			case 20: result = CalcCMMDC(a,b); break;
			case 21: result = CalcCMMMC(a,b); break;
			case 30: result = CalcMin(a,b); break;
			case 31: result = CalcMax(a,b); break;
			case 41: result = CalcA(a,b); break;
			case 42: result = CalcC(a,b); break;
			}

		}

		else //nu este virgula
		{
			if (!(GetNrNrs(m_strExpression, first+1, last-1)==1 && IsSingle(first+1, last-1)))
			{
				goFunc = FALSE;
				Calc(first+1, last-1);
				right = last;
//				if (GetNrNrs(m_strExpression, first+1, last-1)>2) t+=s[last];
			}

			else
			if (m_strExpression[first]!='(' && GetCharType(m_strExpression[first])==4)
			{
				CString nou = GetFunction(m_strExpression, Contor, first-1);
				t+=nou;
				t+='(';
				t+=CalcParan(first, last);
				t+=')';
				Contor = last+1;
				right = last;
				goFunc = FALSE;
			}

			if (!goFunc) { From = left; return _T("");}//AICI
			GetNextNumber(m_strExpression, first, TRUE, b);
			
			switch (FoundFunction(func))
			{
			case 10: result = CalcRad(2, b); break;
			case 12: result = CalcLog(M_E, b); break;
			case 13: result = CalcLog(10, b); break;
			case 40: result = CalcP(b); break;
			}
		}

		right = last;
	}

	else	// rad5
	{
		GetNextNumber(m_strExpression, right+1, TRUE, b);
		GetName(m_strExpression, right+1, left, right);
		left = From;


		switch (FoundFunction(func))
		{
		case 10: result = CalcRad(2,b); break;
		case 12: result = CalcLog(M_E,b); break;
		case 13: result = CalcLog(10,b); break;
		}
	}

	From = right;
	return result;
}

inline CString CExpresion::CalcRad(double a, double b)
{
	double nr;
	CString result;

	if (a!= floor(a) || a<0)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie natural, mai mare sau egal cu 2!";
		return str;
	}
	if (modulo(a,2)==0 && b<=0)
	{
		CString str;
		dtoa(b, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		dtoa(a,result);
		str+= "\" trebuie sã fie pozitiv dacã \"";
		str+= result;
		str+= "\" este par!";
		return str;
	}
	if (a<2)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" Este considerat natural, mai mare sau egal cu 2!";
		return str;
	}

	nr = pow(abs(b), 1/a);
	if (b < 0) nr = -nr;//we are here in the case of rad(3,-8).
	dtoa(nr, result);

	return result;
}

inline CString CExpresion::CalcLog(double a, double b)
{
	double nr;
	CString result;

	if (a<=0 || a==1)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie strict pozitiv ºi diferit de 1!";
		return str;
	}
	if (b<=0)
	{
		CString str;
		dtoa(b, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie strict pozitiv!";
		return str;
	}
	nr = log(b)/log(a);
	dtoa(nr, result);

	return result;
}

inline CString CExpresion::CalcCMMDC(double a, double b)
{
	CString result;
	if (a<0) a=-a;
	if (b<0) b=-b;

	if (a!= floor(a))
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie întreg nenul!";
		return str;
	}
	if (b!= floor(b))
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie intreg nenul!";
		return str;
	}

	while (a!=b)
		if (a>b) a-=b;
		else b-=a;
	dtoa(a, result);

	return result;
}

inline CString CExpresion::CalcCMMMC(double a, double b)
{
	CString result;
	if (a<0) a=-a;
	if (b<0) b=-b;
	double x(a), y(b);

	if (a!= floor(a) || a==0)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie întreg nenul!";
		return str;
	}
	if (b!= floor(b) || b==0)
	{
		CString str;
		dtoa(b, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie întreg nenul!";
		return str;
	}

	while (x!=y)
		if (x>y) x-=y;
		else y-=x;
	dtoa(a/x*b, result);

	return result;
}

inline CString CExpresion::CalcMin(double a, double b)
{
	CString result;

	if (a<b) dtoa(a, result);
	else dtoa(b, result);

	return result;
}

inline CString CExpresion::CalcMax(double a, double b)
{
	CString result;

	if (a>b) dtoa(a, result);
	else dtoa(b, result);

	return result;
}

inline CString CExpresion::CalcP(double n)
{
	CString result;

	if (n!=floor(n) || n<0)
	{
		CString str;
		dtoa(n, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie natural!";
		return str;
	}
	n = perm(n);
	dtoa(n, result);

	return result;
}

inline CString CExpresion::CalcA(double a, double b)
{
	CString result;
	double nr;

	if (a!= floor(a) || a<=0)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie natural nenul!";
		return str;
	}
	if (b!= floor(b) || b<=0)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie natural nenul!";
		return str;
	}
	if (b>a)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie mai mare sau egal cu elementul \"!";
		dtoa(b,result);
		str+= result;
		str+="\".";
		return str;
	};

	nr = perm(a)/perm(a-b);
	dtoa(nr, result);

	return result;
}

inline CString CExpresion::CalcC(double a, double b)
{
	CString result;
	double nr;

	if (a!=floor(a) || a<=0) 
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie natural nenul!";
		return str;
	}
	if (b!= floor(b) || b<=0)
	{
		CString str;
		dtoa(b, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie natural nenul!";
		return str;
	}
	if (b>a)
	{
		CString str;
		dtoa(a, result);
		str+= "NULL Eroare! Elementul \"";
		str+= result;
		str+= "\" trebuie sã fie mai mare sau egal cu \"";
		dtoa(a, result);
		str+= result;
		str+="\".";
		return str;
	}

	nr = perm(a)/(perm(b)*perm(a-b));
	dtoa(nr, result);

	return result;
}

inline double CExpresion::modulo(double a, double b)
{
	if (a!= floor(a)) return -1;
	if (b!= floor(b)) return -2;
	if (b<=0) return -3;

	if (a<0) a = -a;

	return (a-floor(a/b)*b);
}

int CExpresion::GetBegOfExpr(int nEnd)
{
	char paranteze[101];
	int nr(0);

	for (int i=nEnd; i>=0; i--)
	{
		switch (m_strExpression[i])
		{
		case '(' : if (paranteze[nr]!=')') m_Validation.SendError(53, nEnd-i>4 ? (i+nEnd)/2 : i, nEnd); else nr--; break;
		case ')': nr++; if (nr>100) { m_Validation.SendError(51); return 0;}
						else paranteze[nr]=')'; break;


		case '[' : if (paranteze[nr]!=']') m_Validation.SendError(53, nEnd-i>4 ? (i+nEnd)/2 : i, nEnd); else nr--; break;
		case ']': nr++; if (nr>100) { m_Validation.SendError(51); return 0;}
						else paranteze[nr]=']'; break;


		case '{' : if (paranteze[nr]!='}') m_Validation.SendError(53, nEnd-i>4 ? (i+nEnd)/2 : i, nEnd); else nr--; break;
		case '}': nr++; if (nr>100) { m_Validation.SendError(51); return 0;}
						else paranteze[nr]='}'; break;


		case '<' : if (paranteze[nr]!='>') m_Validation.SendError(53, nEnd-i>4 ? (i+nEnd)/2 : i, nEnd); else nr--; break;
		case '>': nr++; if (nr>100) { m_Validation.SendError(51); return 0;}
						else paranteze[nr]='>'; break;
		}

		if (nr==0) return i;
	}

	if (nr) m_Validation.SendError(54);

	return -1;
}


inline BOOL CExpresion::IsNumberParan(int poz, BOOL Forward)
{
	//aaa'('
	if (Forward)
	{
		if (GetNrNrs(m_strExpression, poz, m_Validation.GetEndOfExpr(poz))>1) return FALSE;
		else return TRUE;
	}

	//aaa')'
	else
	{
		int beg = GetBegOfExpr(poz);
		if (GetNrNrs(m_strExpression, beg, poz)>1) return FALSE;
		else {
			int name, left, right;
			GetBkName(m_strExpression, beg, left, right);
			name = FoundFunction(GetFunction(m_strExpression, left, right));
			if (name>9 && name<50) return FALSE;
			else return TRUE;
		}
	}
}

inline void CExpresion::PrepareString()
{
	int From(0), name1(0), name2(0), left, right, poz, l, r;

	while (From<m_strExpression.GetLength()-1)
	{
		name2= GetName(m_strExpression, From, left, right);
		poz = right;

		//daca nu e operator
		if (!IsOperator(m_strExpression, From) && m_strExpression[From]!= ' ' || m_strExpression[From]=='!')
		{
			GetBkName(m_strExpression, right, l, r);
			name1 = FoundFunction(GetFunction(m_strExpression, l, r));
			if (m_strExpression[right+1]==' ') right++;
			name2 = GetName(m_strExpression, right+1, left, right);
			//daca nu e nici aici operator, dar sa nu fie incadrat
			
			if (!(name1<9 && name1>0 && FoundFunction(GetFunction(m_strExpression, left, right))==50))
			if (!IsOperator(m_strExpression, left) && GetCharType(m_strExpression[poz])!=4 &&
				GetCharType(m_strExpression[right])!=5 && name1<9 &&
				m_strExpression[poz]!=',' && m_strExpression[right]!=',' && m_strExpression[right]!='=') 
			{
				m_strExpression.Insert(poz+1, L"*");
				right++;
			}
			From = right;
		}

		else From = poz+1;
	}

}

inline BOOL CExpresion::IsSingle(int From, int To)
{
	if (GetCharType(m_strExpression[From])==1) From++;
	if (m_strExpression[To]=='=') To--;
	int nr(0), name(0), l, r;

	while (From<=To)
	{
		name = GetName(m_strExpression, From, l, r);
		if (m_strExpression[From]==' ') From++;
		else {From = r+1; nr++;}
	}

	if (nr>1) return FALSE;
	else return TRUE;
}

int CExpresion::GetErrorPos1()
{
	return m_Validation.err_pos1;
}

int CExpresion::GetErrorPos2()
{
	return m_Validation.err_pos2;
}

int CExpresion::GetHighBeg()
{
	return m_Validation.HighBeg;
}

int CExpresion::GetHighEnd()
{
	return m_Validation.HighEnd;
}

int CExpresion::CheckValid()
{
	return m_Validation.Validate(m_strExpression);
}