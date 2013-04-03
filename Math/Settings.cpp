#include "StdAfx.h"
#include "Settings.h"

extern TCHAR path[260];

CSettings::CSettings(void)
{
	fisier.Open(L"Settings.dat", CFile::modeCreate | CFile::modeReadWrite |
		CFile::typeBinary | CFile::modeNoTruncate);
}

CSettings::~CSettings(void)
{
}

void CSettings::LoadSettings(bool &ShowResult, unsigned char &nr_zecimale, TCHAR path[260])
{
	fisier.SeekToBegin();
	if (!fisier.Read(&ShowResult, 1))
		ShowResult = false;
	if (!fisier.Read(&nr_zecimale, 1))
		nr_zecimale = 2;
	if (!fisier.Read(path, 520))
		wcscpy(path, L"C:\\Documents and Settings\\Administrator\\My Documents");
}

void CSettings::SaveSettings(bool ShowResult, unsigned char nr_zecimale, TCHAR path[260])
{
	fisier.SeekToBegin();
	fisier.Write(&ShowResult, 1);
	fisier.Write(&nr_zecimale, 1);
	fisier.Write(path, 260*2);
	wcscpy(::path, path);
}