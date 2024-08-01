#include "calc/base/stderr.h"

/* =---- Common Error Output Functions -------------------------= */

#if !_CALC_BUILD_INLINES

int _CDECL error(const char *const message)
{
    return _stderr_error(message);
}

int _CDECL errorln(const char *const message)
{
    return _stderr_errorln(message)
}

int _CDECL verrorf(const char *const format, va_list arglist)
{
    return _stderr_verrorf(format, arglist);
}

int _CDECL verrorfn(const char *const format, va_list arglist)
{
    return _stderr_verrorfn(format, arglist);
}

int _CDECL errorf(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);
    result = _stderr_verrorf(format, arglist);
    va_end(arglist);

    return result;
}

int _CDECL errorfn(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);
    result = _stderr_verrorfn(format, arglist);
    va_end(arglist);

    return result;
}

#endif // _CALC_BUILD_INLINES

int _CDECL fail(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);
    result = _stderr_verrorfn(format, arglist);
    va_end(arglist);

    exit(EXIT_ABORTED);

    return result;
}

/* =------------------------------------------------------------= */
