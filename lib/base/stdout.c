#include "calc/base/stdout.h"

/* =---- Output Functions --------------------------------------= */

#pragma region System Outputting Functions

// +---- Internal (Output Functions) ---------------------------+

#pragma region Internal (Output Functions)

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

static inline int _vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
    return vfprintf(stream, format, arglist) + _fputln(stream);
}

static inline int _vprintfn(const char *const format, va_list arglist)
{
    return vprintf(format, arglist) + _putln();
}

#pragma endregion

// +---- Internal (Output Functions) ---- End ------------------+

int _CDECL fputln(FILE *const stream)
{
    return _fputln(stream);
}

int _CDECL putln()
{
    return _putln();
}

// Print Functions

int _CDECL fprint(FILE *const stream, const char *const message)
{
    return fputs(message, stream);
}

int _CDECL fprintln(FILE *const stream, const char *const message)
{
    return fputs(message, stream) + _fputln(stream);
}

int _CDECL print(const char *const message)
{
    return puts(message);
}

int _CDECL println(const char *const message)
{
    return puts(message) + _putln();
}

int _CDECL vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
    return _vfprintfn(stream, format, arglist);
}

int _CDECL fprintfn(FILE *const stream, const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = _vfprintfn(stream, format, arglist);

    va_end(arglist);

    return result;
}

int _CDECL vprintfn(const char *const format, va_list arglist)
{
    return _vprintfn(format, arglist);
}

int _CDECL printfn(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = _vprintfn(format, arglist);

    va_end(arglist);

    return result;
}

// Error Print Functions

int _CDECL error(const char *const message)
{
    return fprint(stderr, message);
}

int _CDECL errorln(const char *const message)
{
    return fprintln(stderr, message);
}

int _CDECL verrorf(const char *const format, va_list arglist)
{
    return vfprintf(stderr, format, arglist);
}

int _CDECL verrorfn(const char *const format, va_list arglist)
{
    return vfprintfn(stderr, format, arglist);
}

int _CDECL errorf(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = vfprintf(stderr, format, arglist);

    va_end(arglist);

    return result;
}

int _CDECL errorfn(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = vfprintfn(stderr, format, arglist);

    va_end(arglist);

    return result;
}

#pragma endregion

/* =------------------------------------------------------------= */
