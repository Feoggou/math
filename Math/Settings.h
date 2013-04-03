#pragma once

class CSettings
{
private:
	CFile fisier;
public:
	CSettings(void);
	~CSettings(void);

	void LoadSettings(bool &ShowResult, unsigned char &nr_zecimale, TCHAR path[260]);
	void SaveSettings(bool ShowResult, unsigned char nr_zecimale, TCHAR path[260]);
};
