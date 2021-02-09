
/* last version in development: Permanent link: http://rextester.com/LDMX41657 */
/* see also Permanent link: http://rextester.com/MKI66515 */
/* Microsoft (R) C/C++ Optimizing Compiler Version 19.00.23506 for x86 */
/* Marquee Text on ouptut fixed-lenght buffer in Development */
/* Code by Twareintor (2016) */


#ifndef _PRNMARQUEE_H_1908238AAAA092838298280FF0092837289372893
#define _PRNMARQUEE_H_1908238AAAA092838298280FF0092837289372893

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/******************************
function to load the ouptut buffer "wszOut" with the "wszIn" repeatedly starting from nFrame 
in order to create a marquee text on a display with a gap between repeats 
note: function in development; "nGap" must be 0 and you provide the gap in the input text 
*******************************/
void cwPrnMqFrame(wchar_t* wszOut, const wchar_t* wszIn, size_t nCharsOut, size_t nCharsIn, size_t nGap, int nFrame);



#endif/*_PRNMARQUEE_H_1908238AAAA092838298280FF0092837289372893*/

