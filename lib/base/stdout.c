#include "calc/base/stdout.h"

/* =---- Common Output Functions -------------------------------= */

#if !_CALC_BUILD_INLINES

int _CDECL fputln(FILE *const stream)
{
    return (_stdout_fputln(stream));
}

int _CDECL putln()
{
    return (_stdout_putln());
}

int _CDECL fprint(FILE *const stream, const char *const message)
{
    return (_stdout_fprint(stream, message));
}

int _CDECL fprintln(FILE *const stream, const char *const message)
{
    return (_stdout_fprintln(stream, message));
}

int _CDECL print(const char *const message)
{
    return (_stdout_print(message));
}

int _CDECL println(const char *const message)
{
    return (_stdout_println(message));
}

int _CDECL vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
    return (_stdout_vfprintfn(stream, format, arglist));
}

int _CDECL vprintfn(const char *const format, va_list arglist)
{
    return (_stdout_vprintfn(format, arglist));
}

int _CDECL fprintfn(FILE *const stream, const char *const format, ...)
{
    int result;
    va_list arglist;
    
    va_start(arglist, format);
    result = _stdout_vfprintfn(stream, format, arglist);
    va_end(arglist);
    
    return result;
}

int _CDECL printfn(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);
    result = _stdout_vprintfn(format, arglist);
    va_end(arglist);

    return result;
}

#endif // _CALC_BUILD_INLINES

/* =------------------------------------------------------------= */
