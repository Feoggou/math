#include "StdAfx.h"
#include "Validation.h"
#include <math.h>
#include "expUtils.h"

CString Valide = L"+-*/^!()[]{}<>|,=0.123456789 ";

Modul m_moduli[101];

CValidation::CValidation(CString& m_strExpression)
:m_sMain(m_strExpression)
{
	m_sMain = L"";
	m_bStop = FALSE;
	HighBeg = HighEnd = -1;
	err_pos1 = err_pos2 = -1;
}

CValidation::~CValidation(void)
{
}

BOOL CValidation::Validate(CString& m_strExpr)
{
	m_sMain = m_strExpr;
	//we eliminate the spaces so that we would work easier with the string.
	EliminateSpaces();

	//=
	//if there is no "=" sign, we add it.
	if (!EqualSignExists()) m_sMain+='=';
	else
	{
		//if there is at least one "=" sign, we must assure there are not many and that is at the right position!
		int nr(0);
		for (int i=0; i<m_sMain.GetLength(); i++)
		{
			if (m_sMain[i] == '=')
			{
				nr++;
				if (i!=m_sMain.GetLength()-1) {SendError(ERR_EQUALNOTLAST); return FALSE;}
			}
		}
		
		if (nr>1) SendError(ERR_MULTIPLEEQUAL);
	}

	//if an error has been triggered, validation failed.
	if (m_bStop) return FALSE;

	//we must check that the elements exist!
	if (!ElementsExist()) return FALSE;

	//if all elements exist, we check for their arrangement (so we would not calculate something like "5+/3ln*5")
	if (!GoodArranged(0,m_sMain.GetLength()-1)) return FALSE;

	//paranthesis
	int left, right;
	if (int x = ValidateParanthesis(left, right))
	{
		if (x< 5) SendError(50, left, right);
		else SendError(51);
		return FALSE;
	}

	m_strExpr = m_sMain;
	return TRUE;
}

inline void CValidation::EliminateSpaces()
{
	CString t;
	int i;

	//we create a string from the original, that allows not consecutive spaces. (there are only max one between)
	for (i=0; i<m_sMain.GetLength(); i++)
		if (m_sMain[i] == ' ')
		{
			if (i>0 && m_sMain[i-1] != ' ') t+=' ';
		}
		else t += m_sMain[i];

	//we place that resulting string in the original one (We replace the original) and clean "t"
	m_sMain = t;
	t = _T("");
	

	//now we have to delete the unnecessary spaces
	for (i=0; i<m_sMain.GetLength(); i++)
		if (m_sMain[i]==' ')
		{
			//if both the preceding and the following one belong NOT to valid (they are constants/functions)
			//then we allow the space: 
			if (!SearchGroup(Valide, m_sMain[i-1]) &&
				!SearchGroup(Valide, m_sMain[i+1])) t+=' ';
		}
		
		else t+=m_sMain[i];

	//the result string:
	m_sMain = t;
}

inline BOOL CValidation::EqualSignExists()
{
	for (int i = m_sMain.GetLength()-1; i>=0; i--)
		if (m_sMain[i] == '=') return TRUE;
	return FALSE;
}

BOOL CValidation::ElementsExist()
{
	//b = beginning; e = ending;
	int b(0),e(0);
	BOOL Invalid(FALSE), Go(FALSE);

	// Se preia necunoscuta, daca exista
	for (int i=0; i< m_sMain.GetLength(); i++)
	{
		//if there is a character not belonging to string "+-*/^!()[]{}<>|,=0.123456789 "
		//we check to see if it is an unvalid character or function or constant, or other invalid strings
		if (Search(Valide, m_sMain[i])==-1 && 
			m_sMain[i]!='<' && m_sMain[i]!='>')
		{
			//first found erred, so beggining (b) receives the position within the string, only the first time erred
			if (Invalid == FALSE) 
			{
				Invalid = TRUE;
				b = i;
			}
		}
		else 
		{ 
			//after we have received the beginning of the erred string, and we met a valid character,
			//we set this as the last invalid as endpoint
			if (Invalid)
			{
				Invalid = FALSE; 
				Go = TRUE;
				e = i-1;
			}
			else e = 0;
		}

		if (Go)
		{
			//tre sa fie functie/constanta
			//Go specifies that the doubtful string/character is received all. So we must check it!

			//we place b->e in a string and we check to see if it is a valid string (function/constant/modulo)
			if (!FoundFunction(GetFunction(m_sMain, b,e))) {SendError(0, b, e); return FALSE;}
			Go = FALSE;
		}
	}
	return TRUE;
}

BOOL CValidation::GoodArranged(int From, int To)
{
	int Current(From);
	Num Numar={0,0};
	BOOL SubExpresie(FALSE), EFunc(FALSE), Result(TRUE);

	//gets the number of module paranthesis and their position, in the member variable m_moduli
	//if there are no module paranthesis, returns FALSE
	if (GetNrM(m_sMain, Current, To)) 
		//if we have modules, we transform them into < > paranthesis, and check their validity
		//return true if valid.
		if (!ArrangeModules())
		{
			if (!m_bStop) SendError(52);
			return FALSE;
		}

	while (Numar.rightOE<To)
	{//......................

//	Numar.leftOE = Numar.rightOE = 0;              trebuie sa se gaseasca in fiecare varianta

	int x(0), Nume(0);

	Nume = GetName(m_sMain, Current, Numar.leftOE, Numar.rightOE);
	
	//Numar/constanta
	if (Nume==1 || Nume==3 && (FoundFunction(GetFunction(m_sMain, Numar.leftOE, Numar.rightOE)))<3)
	{
		//se verifica daca numarul este valid
		if (Nume==1)
			ValidateNumber(Numar.leftOE, Numar.rightOE);
		if (m_bStop) return FALSE;
		TestRight(Current, m_sMain[Current], To);
		if (m_bStop) return FALSE;
	}

	//Incadrare (Expresie)
	else if (Nume==2)
	{

		//1 () 2[] 3{} 4||

		if (Search(Valide, m_sMain[Current])%2==0)
		{
			SubExpresie = TRUE;
			TestRight(Current, m_sMain[Current], To);
			if (m_bStop) return FALSE;
			if (Result) {
				Result = GoodArranged(Numar.rightOE+1,GetEndOfExpr(Numar.rightOE)-1);
//				Numar.rightOE++;	aici e problema, knd iasa din goodarranged
			}
		}
		else
		{
			TestRight(Current, m_sMain[Current], To);
			if (m_bStop) return FALSE;
			if (!EFunc) {SubExpresie = FALSE;/*Numar.rightOE++;*/}
			else { EFunc = FALSE; Numar.rightOE = Current;}
		}
	}

	//Functzie
	else if (Nume==3)
	{
		int functia = FoundFunction(GetFunction(m_sMain, Numar.leftOE, Numar.rightOE));
		int tip(0);

		//tip 1-bi; 2-1; 3-2;

		//uni sau bi, cu sau fara paranteze pt uni
		if (functia==10) tip = 1;

		//uni, cu sau fara paranteze
		else if (functia==12 || functia==13) tip = 2;

		//bi, cu paranteze
		else if (functia == 11 || functia>=20 && functia !=40 && functia !=50) tip = 3;

		//uni, cu paranteze
		else if (functia ==40) tip = 4;

		//operatorul mod ... fara paranteze obligatorii
		else if (functia ==50) tip = 5;


		//RAD
		if (tip==1)
		{
			//daca incepe cu paranteza
			if (GetCharType(m_sMain[Numar.rightOE+1])==4)
			{
				int y = GetEndOfExpr(Numar.rightOE+1);
				int x = SearchVPos(Numar.rightOE+2, y-1);

				if (!y) return FALSE;

				//daca este virgula
				if (x>-1)
				{
					if (m_sMain[Numar.rightOE+1]!='(')
					{
						SendError(5, Numar.leftOE, GetEndOfExpr(Numar.rightOE+1));
						return FALSE;
					}
					TestRight(Numar.rightOE+1,m_sMain[Numar.rightOE+1], x);
					if (m_bStop) return FALSE;
					if (Result) Result = GoodArranged(Numar.rightOE+2, x-1);
					TestRight(x,m_sMain[x], x);
					if (m_bStop) return FALSE;
					if (Result) Result = GoodArranged(x+1, y-1);
				}
				else { if (Result) Result = GoodArranged(Numar.rightOE+2, y-1);}

				EFunc = TRUE;
				Numar.rightOE++;
			}

			//daca nu incepe cu paranteza
			else 
			{
				if (SearchGroup(Valide, m_sMain[Numar.rightOE+1])!=5)
				{
					SendError(1, Numar.rightOE+1, Numar.rightOE+1);
					return FALSE;
				}
				GetName(m_sMain, Numar.rightOE+1, Numar.leftOE, Numar.rightOE);
				if (Result) Result = GoodArranged(Numar.leftOE, Numar.rightOE);
			}
		}

		//LN, LG
		else if (tip==2)
		{
			//daca incepe cu paranteza
			if (GetCharType(m_sMain[Numar.rightOE+1])==4)
			{
				int y = GetEndOfExpr(Numar.rightOE+1);
				int x = SearchVPos(Numar.rightOE+2, y-1);

				if (!y) return FALSE;

				//daca este virgula
				if (x>-1) {SendError(2, Numar.leftOE, y);return FALSE;}

				if (Result) Result = GoodArranged(Numar.rightOE+2, y-1);
				EFunc = TRUE;
				Numar.rightOE++;
			}

			//daca nu incepe cu paranteza
			else 
			{
				if (SearchGroup(Valide, m_sMain[Numar.rightOE+1])!=5)
				{
					SendError(1, Numar.rightOE+1, Numar.rightOE+1);
					return FALSE;
				}
				GetName(m_sMain, Numar.rightOE+1, Numar.leftOE, Numar.rightOE);
				if (Result) Result = GoodArranged(Numar.leftOE, Numar.rightOE);
			}
		}

		//CMMDC, CMMMC, MIN, MAX, A, C, log
		else if (tip==3)
		{
			//daca incepe cu paranteza
			if (GetCharType(m_sMain[Numar.rightOE+1])==4)
			{
				int y = GetEndOfExpr(Numar.rightOE+1);
				int x = SearchVPos(Numar.rightOE+2, y-1);

				if (!y) return FALSE;

				//daca este virgula
				if (x>-1)
				{
					if (m_sMain[Numar.rightOE+1]!='(')
					{
						SendError(5, Numar.leftOE, GetEndOfExpr(Numar.rightOE+1));
						return FALSE;
					}
					TestRight(Numar.rightOE+1,m_sMain[Numar.rightOE+1], x);
					if (m_bStop) return FALSE;
					if (Result) Result = GoodArranged(Numar.rightOE+2, x-1);
					TestRight(x,m_sMain[x], x);
					if (m_bStop) return FALSE;
					if (Result) Result = GoodArranged(x+1, y-1);
				}
				else {SendError(3, Numar.leftOE, y); return FALSE;}
				EFunc = TRUE;
				Numar.rightOE++;
			}

			//daca nu incepe cu paranteza
			else {SendError(4, Numar.leftOE, Numar.rightOE); return FALSE;}
		}

		//P
		else if (tip==4)
		{
			//daca incepe cu paranteza
			if (GetCharType(m_sMain[Numar.rightOE+1])==4)
			{
				int y = GetEndOfExpr(Numar.rightOE+1);
				int x = SearchVPos(Numar.rightOE+2, y-1);

				if (!y) return FALSE;

				//daca este virgula
				if (x>-1) {SendError(2, Numar.leftOE, x); return FALSE;}

				if (Result) Result = GoodArranged(Numar.rightOE+2, y-1);
				EFunc = TRUE;
				Numar.rightOE++;
			}
			//daca nu incepe cu paranteza
			else {SendError(4, Numar.leftOE, Numar.rightOE); return FALSE;}
		}
		
		//MOD
		else if (tip==5) TestRight(Current, m_sMain[Current], To);
		if (m_bStop) return FALSE;
	}

	//Operatzie
	else if (Nume==4)
	{
		TestRight(Current, m_sMain[Current], To);
		if (m_bStop) return FALSE;
	}

	//Egal =
	else if (Nume==5)
		if (Numar.rightOE+1!=m_sMain.GetLength()) {SendError(10); return FALSE;}

	if (!SubExpresie && !EFunc) Current = Numar.rightOE+1;
	else Current = GetEndOfExpr(Numar.rightOE);
	}//........................  End of While

	return TRUE;
}

int CValidation::ValidateParanthesis(int& left, int& right)
{

	//tip: () = 1, [] = 2, {} = 3, <> = 4, corect = 0, >nrMax 5

	int paran[103];
	int nr(0);
	left = right = -1;

	for (int i=0; i<m_sMain.GetLength()-1; i++)
		switch (m_sMain[i])
	{
		case '(': nr++; if (nr>100) return 5; paran[nr] = i; break;
		case '[': nr++; if (nr>100) return 5; paran[nr] = i; break;
		case '{': nr++; if (nr>100) return 5; paran[nr] = i; break;
		case '<': nr++; if (nr>100) return 5; paran[nr] = i; break;

		case ')': nr++; if (nr>102) return 5;
				  paran[nr] = i;
				  if (m_sMain[paran[nr-1]]!=')') { paran[nr] = i; nr--;}
					else if (paran[nr] == paran[nr-1]+1 &&
						paran[nr-2] == paran[nr-3]+1)
					{
						left = paran[nr-3];
						right = paran[nr];
						return 1;
					}
					else if (m_sMain[paran[nr-1]]==')') nr-=2;
					else nr--; break;

		case ']': nr++; if (nr>102) return 5;
				   paran[nr] = i;
				   if (paran[nr-1]!=']') {paran[nr] = i; nr--;}
					else if (paran[nr] == paran[nr-1]+1 &&
							 paran[nr-2] == paran[nr-3]+1)
					{
						left = paran[nr-3];
						right = paran[nr];
						return 2;
					}
					else if (m_sMain[paran[nr-1]]==']') nr-=2;
					else nr--; break;

		case '}': nr++; if (nr>102) return 5;
				   paran[nr] = i;
				   if (paran[nr-1]!='}') {paran[nr] = i; nr--;}
					else if (paran[nr] == paran[nr-1]+1 &&
							 paran[nr-2] == paran[nr-3]+1)
					{
						left = paran[nr-3];
						right = paran[nr];
						return 3;
					}
					else if (m_sMain[paran[nr-1]]=='}') nr-=2;
					else nr--; break;

		case '>': nr++; if (nr>102) return 5;
				   paran[nr] = i;
				   if (paran[nr-1]!='>') {paran[nr] = i; nr--;}
					else if (paran[nr] == paran[nr-1]+1 &&
							 paran[nr-2] == paran[nr-3]+1)
					{
						left = paran[nr-3];
						right = paran[nr];
						return 4;
					}
					else if (m_sMain[paran[nr-1]]=='>') nr-=2;
					else nr--; break;
	}

	return 0;
}

inline int CValidation::ValidateNumber(int left, int right)
{
	int nErrors(0), nr(0);

	for (int i=left; i<=right; i++)
		if (m_sMain[i]=='.') nr++;

	if (nr>1) {SendError(20, left, right); nErrors++; }
	if (m_sMain[left]=='.') { SendError(21, left, right); nErrors++;}
	else if (m_sMain[left]=='0' && m_sMain[left+1]!='.' && right!=left)
		{SendError(22, left, right); nErrors++;}
	if (m_sMain[right]=='.') {SendError(23, left, right); nErrors++;}

	if (!nErrors)
	{
		double nrD;
		nrD = _wtof(GetFunction(m_sMain, left, right));
		nrD = floor(nrD);
		if (nrD>999999999999) {SendError(24, left, right); nErrors++;}
	}

	return nErrors;
}

inline void CValidation::TestRight(int poz, WCHAR c, int To)
{
	int x= GetCharType(c), y, left, right,l ,r, nr;

	//current left, right; urmat l, r
	GetName(m_sMain,poz, left, right);
	int z = FoundFunction(GetFunction(m_sMain, left, right));
	y = GetCharType(m_sMain[right+1]);
	if (y==10)
	{
		y = GetCharType(m_sMain[right+2]);
		GetName(m_sMain, right+2, l, r);
		nr = right+2;
	} else {GetName(m_sMain, right+1, l, r); nr = right+1;}


	//inceputul se face separat in functzia asta
	//virgula, punctul
	//la functzie, in dreapta, nu se rezolva aici...

	// testeaza doar c e in dreapta lui
	//'!', numar si ')' pot avea orice in dreapta

	//const
	if (x==0)
	{
		//mod
		if (z==50)
			{
			if (!(y==4 || y==9|| y==0)) SendError(30, left, nr);

				//mod
			if (FoundFunction(GetFunction(m_sMain, l, r))==50) SendError(30, left, nr);
			}
		else 
			if (y==7 && l!=To+1) SendError(40, left, nr);

			if (m_sMain[l]=='.') SendError(41, left, nr);
	}

	//operatori binari */^ sau +-
	else if (x==2 || x==1)
	{
		if (!(y==4 || y==9|| y==0)) SendError(30, left, nr);
		if (m_sMain[l]=='.') SendError(41, left, nr);
		if (FoundFunction(GetFunction(m_sMain, l,r))==50) SendError(30, left, nr);
	}

	// '(', ','
	else if (x==4 || x==7)
	{
		if (!(y==1 || y==4 || y==9|| y==0)) SendError(30, left, nr);
		if (m_sMain[l]=='.') SendError(41, left, nr);
		if (FoundFunction(GetFunction(m_sMain, l,r))==50) SendError(30, left, nr);
	}

	else 
		if (m_sMain[l]==',' && l!=To+1) SendError(40, left, nr);

	//la inceput
	if (poz==0)
	{
		if (!(x==1 || x==4 || x==9|| x==0)) SendError(30, left, nr);
		if (m_sMain[l]=='.') SendError(41, left, nr);//x!=9, adica numar, pt k '.' este parte d numar
		if (FoundFunction(GetFunction(m_sMain, left,right))==50) SendError(30, left, nr);
	}
}


int CValidation::GetEndOfExpr(int nFirst)
{
	char paranteze[101];
	int nr(0);
//	if (!s[nFirst]) return 0;

	for (int i=nFirst; i<m_sMain.GetLength(); i++)
	{
		switch (m_sMain[i])
		{
		case '(' : nr++; if (nr>100) { SendError(51); return 0;}
						else paranteze[nr]='('; break;
		case ')': if (paranteze[nr]!='(')
				  {
					  SendError(53, i-nFirst>4 ? (nFirst+i)/2 : nFirst, i);
					  return 0;
				  }
				else nr--;break;


		case '[' : nr++; if (nr>100) { SendError(51); return 0;}
						else paranteze[nr]='['; break;
		case ']': if (paranteze[nr]!='[')
				  {
					  SendError(53, i-nFirst>4 ? (nFirst+i)/2 : nFirst, i);
					  return 0;
				  }
				else nr--;break;


		case '{' : nr++; if (nr>100) { SendError(51); return 0;}
						else paranteze[nr]='{'; break;
		case '}': if (paranteze[nr]!='{')
				  {
					  SendError(53, i-nFirst>4 ? (nFirst+i)/2 : nFirst, i);
					  return 0;
				  }
				else nr--;break;


		case '<' : nr++; if (nr>100) { SendError(51); return 0;}
						else paranteze[nr]='<'; break;
		case '>': if (paranteze[nr]!='<')
				  {
					  SendError(53, i-nFirst>4 ? (nFirst+i)/2 : nFirst, i);
					  return 0;
				  }
				else nr--;break;
		}

		if (nr==0) return i;
	}

	if (nr) SendError(54);

	return 0;
}

int CValidation::SearchVPos(int From, int To)
{
	int i = From, left, right;

	while (i<To)
	{
		if (m_sMain[i]==',') return i;
		GetName(m_sMain, i, left, right);
		if (IsFunction(m_sMain, i) && GetCharType(m_sMain[right+1])==4)
		{
			if (SearchVPos(right+2, GetEndOfExpr(right+1)-1)>-1)
				right = SearchVPos(right+2, GetEndOfExpr(right+1)-1);
			else right = GetEndOfExpr(right+1);
		}
		i=right+1;
	}

	return -1;
}

BOOL CValidation::ArrangeModules()
{
	int k,i;
	BOOL inapoi(FALSE);
	Modul good[101];
	int nrM = GetNrM(m_sMain, 0, m_sMain.GetLength()-1);

	if (!nrM) return FALSE;

	//we have for each module, it's position, but we have not their destination! (where they end). 
	//we initialize that with 0
	for (i=0; i<= nrM; i++) m_moduli[i].dest=0;
	nrVarPos=0;
	k=1;

	//backtracking to check the possibilities
	//k represents position
	while (k>0)
	{
		if (k==nrM+1)
		{
			//if we riched the end, then it means we have one more variant that can be true!
			nrVarPos++;
			for (i=1; i<k; i++) good[i] = m_moduli[i]; //and we store the value in "good" array
			k--;
		}

		else if (m_moduli[k].dest<nrM)
		{
			//destination must point to the position of another module, which must not step over maximum
			{
				//we check if it works pointing to the next one.
				m_moduli[k].dest++;

				//if 2 modules point to ecah other and our position points the last module, then we go to
				//the next variant!
				if (m_moduli[m_moduli[k].dest].dest==k && k>m_moduli[k].dest) k++;
				else
					//if not, check for validation
				if (Continue(k))
				{
					k++;
					inapoi = FALSE;
				}
			}
		}

		else 
		{
			m_moduli[k].dest=0;
			k--;
			inapoi = TRUE;
		}
	}

	if (nrVarPos>1) return FALSE;
	if (nrVarPos==0) {SendError(55, 0,0); return FALSE;}

	for (i=1; i<=nrM; i++) m_moduli[i] = good[i];
	for (i=1; i<=nrM; i++)
		if (i<m_moduli[i].dest)
		{
			m_sMain.Delete(m_moduli[i].poz,1);
			m_sMain.Insert(m_moduli[i].poz, L"<");
		}
		else 
		{
			m_sMain.Delete(m_moduli[i].poz,1);
			m_sMain.Insert(m_moduli[i].poz, L">");
		}

	return TRUE;
}

BOOL CValidation::Continue(int k)
{
	int i,j, x, y;

	//nu se autodetermina termenii
	if (m_moduli[k].dest == k) return FALSE;

	//trebuie sa fie gasite toate variantele pana la k
	for (i=1; i<=k; i++) if (m_moduli[i].dest==0) return FALSE;

	//nu trebuie sa se repete termenii
	for (i=1; i<k; i++)
		for (j=i+1; j<=k; j++)
			if (m_moduli[i].dest==m_moduli[j].dest) return FALSE;

	//dest unui elem nu trebuie sa aiba alta dest dak elem>dest lui

	for (i=1; i<=k; i++)
		if (i>m_moduli[i].dest && m_moduli[m_moduli[i].dest].dest!=i) return FALSE;

	//fiecarui termen ii este asociat un altul de paritate diferita
	for (i=1; i<=k; i++)
		if (i%2==0 && m_moduli[i].dest%2==0 ||
			i%2!=0 && m_moduli[i].dest%2!=0) return FALSE;

	//trebuie sa existe minimun 1 element intre moduli
	for (i=1; i<=k; i++)
	{
		if (i<m_moduli[i].dest && m_moduli[i].poz+1==m_moduli[m_moduli[i].dest].poz) return FALSE;
		else if (i>m_moduli[i].dest && m_moduli[i].poz==m_moduli[m_moduli[i].dest].poz+1) return FALSE;
	}

	//modulii trebuie sa fie inclusi in moduli
	for (i=1; i<=k; i++)
		for (j=1; j<i; j++)
			if (m_moduli[j].dest>i && m_moduli[i].dest>m_moduli[j].dest && i>j) return FALSE;

	//checks if the expression within these modules is valid (so we would not have things like |5+3|56-7|)
	for (i=1; i<=k; i++)
	{
		x = m_moduli[i].poz;
		y = m_moduli[m_moduli[i].dest].poz;
		if (x<y)
		{
			if (!TestExpression(x, y, 1)) return FALSE;
		}
		else if (!TestExpression(y, x, 1)) return FALSE;
	}

	return TRUE;
}

inline BOOL CValidation::TestExpression(int x, int y, int c)
{

	// nu trebuie sa fie (]...{) paranteza principala fiind (...), nici altceva c nu trebuie sa fie
	//(...
	int paranx= SearchGroup(Valide,m_sMain[x+1]);
	int parany= Search(Valide,m_sMain[x+1]);
	if (paranx == SHGROUP_PARANTHESIS && parany%2!=0 && m_sMain[x+1]!='|') return FALSE;
	if (IsOperator(m_sMain, x+1) && GetCharType(m_sMain[x+1])!=1 || m_sMain[x+1]==',') return FALSE;

	//...)
	paranx= SearchGroup(Valide,m_sMain[y-1]);
	parany= Search(Valide,m_sMain[y-1]);
	if (paranx==2 && parany%2==0 && m_sMain[y-1]!='|') return FALSE;
	{ int l, r, name; GetBkName(m_sMain, y-1, l, r); name = FoundFunction(GetFunction(m_sMain, l, r));
	if (IsOperator(m_sMain, y-1) && m_sMain[y-1]!='!' || m_sMain[y-1]==',' || name>9) return FALSE;
	}

	//dak se ia in conisderare fara (+ doar pt modul
	if (c==1 && m_sMain[x+1]=='+') return FALSE;

	// dak parantezele din interior sunt corect aranjate, distribuite
	if (!GetParanArr(x,y)) return FALSE;

	return TRUE;
}


BOOL CValidation::GetParanArr(int From, int To)
{
	if (From<0 || From>m_sMain.GetLength()-1 || From>To)
		return FALSE;

	char paran[101];
	int nr(0);

	for (int i=From; i<= To; i++)
		switch (m_sMain[i])
	{
		case '(' : nr++; if (nr>100) { SendError(51); return FALSE;}
						else paran[nr] = '('; break;
		case '[' : nr++; if (nr>100) { SendError(51); return FALSE;}
						else paran[nr] = '['; break;
		case '{' : nr++; if (nr>100) { SendError(51); return FALSE;}
						else paran[nr] = '{'; break;
		case '<' : nr++; if (nr>100) { SendError(51); return FALSE;}
						else paran[nr] = '<'; break;

		case ')' : if (paran[nr] != '(') return FALSE;
					else nr--; break;
		case ']' : if (paran[nr] != '[') return FALSE;
					else nr--; break;
		case '}' : if (paran[nr] != '{') return FALSE;
					else nr--; break;
		case '>' : if (paran[nr] != '<') return FALSE;
					else nr--; break;
	}

	if (nr) return FALSE;

	return TRUE;
}

inline void CValidation::SendError(int errorN, int From, int To)
{
	CString text;

	m_bStop = TRUE;
//	m_sMain+="\r\n\r\n\t";
	m_sMain+="\n\n\t";
	int lastlength =  m_sMain.GetLength();
	text = GetFunction(m_sMain, From, To);
	switch (errorN)
	{

	//Functzie
	case 0: m_sMain += "Eroare! Funcþia/constanta \"";
			m_sMain += text;
			m_sMain += "\" nu existã!"; break;

	case 1: m_sMain += "Eroare! Dupã numele funcþiei se aºteaptã un numãr sau o expresie în parantezã: \"";
			m_sMain += text; 
			m_sMain += L"\"";break;

	case 2: m_sMain += "Eroare! Caracterul \",\" este aºezat greºit. Funcþia acceptã doar un singur parametru: \"";
			m_sMain += text; 
			m_sMain += L"\"";break;

	case 3: m_sMain += "Eroare! Caracterul \",\" este obligatoriu. Nu a fost gãsit: \"";
			m_sMain += text; 
			m_sMain += L"\""; break;

	case 4: m_sMain += "Eroare! Funcþia \"";
			m_sMain += text;
			m_sMain += "\" are nevoie de parantezã. Nu a fost gãsitã!";
			break;

	case 5: m_sMain += "Eroare! Funcþiile cu douã argumente acceptã doar paranteze rotunde: \"";
			m_sMain += text;
			m_sMain += "\"!";
			break;


	//Egal
	case ERR_EQUALNOTLAST: m_sMain += "Eroare! Nu trebuie sã fie scris nimic dupã \"=\" !"; break;
	case ERR_MULTIPLEEQUAL: m_sMain += "Eroare! Au fost gãsite mai multe caractere \"=\". Trebuie sã fie doar unul!";
			 break;


	//Numar
	case 20: m_sMain += "Eroare! Numãrul \"";
			 m_sMain += text;
			 m_sMain += "\" este invalid. Conþine mai multe puncte de despãrþire cãtre partea fracþionalã!";
			 break;

	case 21: m_sMain += "Eroare! Numãrul \"";
			 m_sMain += text;
			 m_sMain += "\" este invalid. Nu trebuie sã înceapã cu \".\"!"; break;

	case 22: m_sMain += "Eroare! Numãrul \"";
			 m_sMain += text;
			 m_sMain += "\" este invalid. Nu poate începe cu \"0\" decât dacã urmeazã partea fracþionalã!";
			 break;

	case 23: m_sMain += "Eroare! Numãrul \"";
			 m_sMain += text;
			 m_sMain += "\" este invalid. Nu trebuie sã se termine cu \".\"!";
			 break;

	case 24: m_sMain += "Eroare! Numãrul \"";
			 m_sMain += text;
			 m_sMain += "\" este format din prea multe cifre. \
Foloseºte numere cu cel mult 11 cifre la partea întreagã!";
			 break;


	//Operatori, aranjare
	case 30: m_sMain += "Eroare! Caracterul este aºezat greºit dupã precedentul: \"";
			 m_sMain += text;
			 m_sMain += "\"!";break;


	//caractere...
	case 40: m_sMain += "Eroare! Virgula este pusã greºit: \"";
			 m_sMain += text;
			 m_sMain += "\"!"; break;

	case 41: m_sMain += "Erorare! Punctul este pus greºit, nu e numãr : \"";
			 m_sMain += text;
			 m_sMain += "\"!"; break;


	//paranteze
	case 50: m_sMain += "Eroare! A fost gãsitã o incadrare în paranteze/moduli multiplã\
la aceeaºi expresie/element: \"";
			 m_sMain += text;
			 m_sMain += "\"!"; break;

	case 51: m_sMain += "Eroare! Ai folosit un numãr prea mare de paranteze. Incearcã sa reduci numãrul lor!";
			 break;

	case 52: m_sMain += "Eroare! Au fost gãsite mai multe variante de încadrãri pentru moduli.\
Incearcã sa specifici varianta corectã folosind paranteze sau operatorul \"*\"!";
			break;

	case 53: m_sMain += "Eroare! Este aºteptat un alt tip de parantezã închisã: \"";
			 m_sMain += text;
			 m_sMain += "\"!"; break;

	case 54: m_sMain += "Eroare! O parantezã a rãmas deschisã!";
			 break;

	case 55: m_sMain += "Eroare! Nu a fost gasitã nici o variantã de corespondenþã între moduli (care modul este închis pentru altul). Încercaþi\
sã folosiþi paranteze, semnul înmulþire \"*\" ºi sã vã asiguraþi cã expresia este corectã!";
			 break;
		}
	HighBeg = From; HighEnd = To+1;

	err_pos1 = lastlength-2;
//	m_sMain +="\r\n";
	m_sMain +="\n";
	err_pos2 =  m_sMain.GetLength() - 1;
}