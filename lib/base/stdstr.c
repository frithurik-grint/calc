#include "calc/base/stdstr.h"

/* =---- String Operatations Functions -------------------------= */

#pragma region String Operatations Functions

_API bool_t _CDECL streq(const char *const str1, const char *const str2)
{
    if (str1 == str2)
        return TRUE;

    if (!str1 || !str2)
        return FALSE;

    size_t len = strlen(str1);

    if (len != strlen(str2))
        return FALSE;

    for (; len > 0; len--)
    {
        if (str1[len] != str2[len])
            return FALSE;
        else
            continue;
    }

    return TRUE;
}

_API bool_t _CDECL strieq(const char *const str1, const char *const str2)
{
    if (str1 == str2)
        return TRUE;

    if (!str1 || !str2)
        return FALSE;

    size_t len = strlen(str1);

    if (len != strlen(str2))
        return FALSE;

    for (; len > 0; len--)
    {
        if ((str1[len] != str2[len]) && (str1[len] != (tolower(str2[len]))))
            return FALSE;
        else
            continue;
    }

    return TRUE;
}

_API char *_CDECL strntolower(char *const dest, const char *const source, size_t length)
{
    char *buf;

    if (!dest)
        buf = stralloc(length);
    else
        buf = dest, buf[length + 1] = '\0';

    size_t i;

    for (i = length; i > 0; i--)
        buf[i] = tolower(source[i]);

    return buf;
}

_API char *_CDECL strtolower(char *const dest, const char *const source)
{
    return strntolower(dest, source, strlen(source));
}

_API char *_CDECL strntoupper(char *const dest, const char *const source, size_t length)
{
    char *buf;

    if (!dest)
        buf = stralloc(length);
    else
        buf = dest, buf[length + 1] = '\0';

    size_t i;

    for (i = length; i > 0; i--)
        buf[i] = tolower(source[i]);

    return buf;
}

_API char *_CDECL strtoupper(char *const dest, const char *const source)
{
    return strntoupper(dest, source, strlen(source));
}

_API char *_CDECL strndcpy(char *const dest, const char *const source, size_t length)
{
    char *buf;

    if (!dest)
        buf = stralloc(length);
    else
        buf = dest, buf[length] = '\0';

    return strncpy(buf, source, length);
}

_API char *_CDECL strdcpy(char *const dest, const char *const source)
{
    return strndcpy(dest, source, strlen(source));
}

_API char *_CDECL unesc(char *const dest, int c)
{
    char buf[3] = { '\\', '\0', '\0' };

    switch (c)
    {
    case '\a':
        buf[1] = 'a';
        break;

    case '\b':
        buf[1] = 'b';
        break;

    case '\f':
        buf[1] = 'f';
        break;

    case '\n':
        buf[1] = 'n';
        break;

    case '\r':
        buf[1] = 'r';
        break;

    case '\t':
        buf[1] = 't';
        break;

    case '\v':
        buf[1] = 'v';
        break;

    case '\\':
        buf[1] = '\\';
        break;

    case '\'':
        buf[1] = '\'';
        break;

    case '\"':
        buf[1] = '"';
        break;

    case '\?':
        buf[1] = '?';
        break;

    case '\0':
        buf[1] = '0';
        break;

    default:
        buf[0] = c;
        break;
    }

    return strdcpy(dest, buf);
}

_API char *_CDECL vformat(char *const dest, const char *const format, va_list arglist)
{
    static char buf[BUFSIZ] = { 0 };

    if (!format)
        return "";

    int count;

    if ((count = vsprintf(buf, format, arglist)) > 0)
        return strndcpy(dest, buf, count);
    else
        return "";
}

_API char *_CDECL format(char *const dest, const char *const format, ...)
{
    char *result;
    va_list arglist;

    va_start(arglist, format);
    result = vformat(dest, format, arglist);
    va_end(arglist);

    return result;
}

_API char *_CDECL vformatn(const char *const format, va_list arglist)
{
    return vformat(NULL, format, arglist);
}

_API char *_CDECL formatn(const char *const format, ...)
{
    char *result;
    va_list arglist;

    va_start(arglist, format);
    result = vformatn(format, arglist);
    va_end(arglist);

    return result;
}

#pragma endregion

/* =------------------------------------------------------------= */
