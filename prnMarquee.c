

#include "prnMarquee.h"


void cwPrnMqFrame(wchar_t* wszOut, const wchar_t* wszIn, size_t nCharsOut, size_t nCharsIn, size_t nGap, int nFrame)
{
	while(wszIn[nCharsIn--]); nCharsIn++; /* this removes the null characters at the end of the input string */
	wmemset(wszOut, L' ', nCharsOut); /* filling with blanks the whole display buffer */
	nFrame+=nCharsOut*(nFrame<0); /* ... then do this to ensure a positive nFrame */
	size_t nOutStart = 0; /* start character where to write in output string */
	size_t nInStart = nFrame%nCharsIn; /* start character to be written in the input string */
	size_t nCharsWri = nCharsIn-nInStart; /* total characters to be written */

	if(nCharsWri>nCharsOut) nCharsWri = nCharsOut;
	wmemcpy(wszOut+nOutStart, wszIn+nInStart, nCharsWri); /* fill the output buffer with the first section or input */

	while(nOutStart<nCharsOut) /* as long as is still something to write in the output buffer..  */
	{
		nOutStart+=(nCharsWri+nGap); /* the gap introduced at the end of the previous written section */
		nInStart = 0; /* and the input string will be written at the very beginning */
		nCharsWri = nCharsIn; /* to be written as many characters as in the input string, but... */
		if(nOutStart+nCharsWri>nCharsOut) nCharsWri = nCharsOut-nOutStart; /* limit till to the  end of the buffer */
		if(nOutStart<=nCharsOut) wmemcpy(wszOut+nOutStart, wszIn+nInStart, nCharsWri); /* next chunk written */
	}
	wszOut[nCharsOut] = L'\0'; /* to ensure the display is a buffer null-terminated */
							   /* wprintf(L"%02d\t", nFrame); *//* for debug only */
}

/***************************************************************************************************************/

