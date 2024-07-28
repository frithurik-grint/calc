#include "calc-liber.h"

// +---- System Dependent Definitions -------------------------+

#pragma region System Dependent Definitions



#pragma endregion

// +---- System Dependent Definitions -- End ------------------+

// +---- System Outputting Functions --------------------------+

#pragma region System Outputting Functions

// +---- Internal

#pragma region Internal

static inline int _fputln(FILE *const stream)
{
#ifdef _CALC_USE_CRLF
    return fputs("\r\n", stream);
#else
    return fputc('\n', stream), 1;
#endif // _CALC_USE_CRLF
}

static inline int _putln()
{
#ifdef _CALC_USE_CRLF
    return puts("\r\n");
#else
    return putchar('\n'), 1;
#endif // _CALC_USE_CRLF
}

#pragma endregion

// +---- Internal -- End

_API int _CDECL fputln(FILE *const stream)
{
    return _fputln(stream);
}

_API int _CDECL putln()
{
    return _putln();
}

#pragma endregion

// +---- System Outputting Functions --- End ------------------+ 

#ifndef CALC_LIBER_C_
#   define CALC_LIBER_C_
#endif // CALC_LIBER_C_
