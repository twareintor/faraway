
#include "cwsprn2.h"

void cwsprn2(wchar_t* wszOut, const wchar_t* wszIn, size_t nOutLen, size_t nInLen, int nOffs)
{
	if(nOffs>nOutLen) nOffs = nOutLen-1;
	if(nOffs+nInLen>nOutLen) nInLen = nOutLen-nOffs;
	wmemset(wszOut, L' ', nOutLen);
	wszOut[nOutLen-1] = L'\0';
	wmemcpy(wszOut+nOffs, wszIn, nInLen);
	wszOut[nOutLen-1] = L'\0';

}

/************************  usage example:

	wchar_t* wszMsg = new wchar_t[0xff];
	// wprintf(L"\"%*ls\"\n", 10, L"the quick brown fox");
	const wchar_t* wszTmp = L"Point(1.4, 32.4), Point(32.8, 44.9), Point(23.4, 29.2)";
	for(int i=-0x10; i<0x40; i++)
	{
		cwsprn2(wszMsg, wszTmp, 0x60, wcslen(wszTmp), i);
		wprintf(L"\"%ls\"\n", wszMsg);
	}
	delete[] wszMsg;
	printf("Hello, world!\n");


**************************************/

