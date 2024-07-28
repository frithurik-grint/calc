#include "calc-liber.h"

// +---- System Dependent Definitions -------------------------+

#pragma region System Dependent Definitions



#pragma endregion

// +---- System Dependent Definitions -- End ------------------+

// +---- System Outputting Functions --------------------------+

#pragma region System Outputting Functions

_API int fputln(FILE *const stream)
{
#ifdef _CALC_USE_CRLF
    return fputs("\r\n", stream);
#else
    return fputc('\n', stream), 1;
#endif // _CALC_USE_CRLF
}

#pragma endregion

// +---- System Outputting Functions --- End ------------------+ 

#ifndef CALC_LIBER_C_
#   define CALC_LIBER_C_
#endif // CALC_LIBER_C_
