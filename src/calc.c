#ifndef CALC_C_
#   define CALC_C_
#endif // CALC_C_

#include "calc.h"

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

// +---- Internal (Memory Checks) ------------------------------+

#pragma region Internal (Memory Checks)

static inline void *_safety_abort(
#ifdef _DEBUG
    const char *const funcname
#endif // _DEBUG
)
{
#ifdef _DEBUG
    fail("error: function %s cannot allocate memory", funcname);
#else
    fail("error: cannot allocate enough memory");
#endif // _DEBUG

    return NULL;
}

static inline void *_safety_check(
#ifdef _DEBUG
    const char *const funcname,
#endif // _DEBUG
    void *block
)
{
    if (!block)
        return _safety_abort(
#ifdef _DEBUG
            funcname
#endif // _DEBUG
        );
    else
        return block;
}

static inline void *_zerofy_block(void *block, size_t size)
{
    assert(block != NULL);

    char *p = (char *)block;

    for (; size > 0; size--)
        p[size - 1] = 0x00;

    return (void *)p;
}

#pragma endregion

// +---- Internal (Memory Checks) --------------------- END ----+

CALC_API void *CALC_CDECL calc_malloc(size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        malloc(size)
    );
}

CALC_API void *CALC_CDECL calc_calloc(size_t count, size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        calloc(count, size)
    );
}

CALC_API void *CALC_CDECL calc_malloc_z(size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        malloc(size)
    ), size);
}

CALC_API void *CALC_CDECL calc_calloc_z(size_t count, size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        calloc(count, size)
    ), count * size);
}

#pragma endregion

/* =---- Data Management ---------------------------------------= */

#pragma region Data Management

// +---- String Operations -------------------------------------+

#pragma region String Operations

CALC_API char *CALC_CDECL strnget(char *const dst, const char *const src, size_t count)
{
    if (!count)
        return dimz(char, 1);

    char *r;

    if (!dst)
        r = dimz(char, count + 1);
    else
        r = dst;

    r[count] = NUL;

    return (!src) ? r : strncpy(r, src, count);
}

CALC_API char *CALC_CDECL strnlow(char *const dst, const char *const src, size_t count)
{
    if (!count)
        return (char *)src;

    char *r = strnget(dst, (const char *)NULL, count);

    do
        r[count - 1] = tolower(src[count - 1]);
    while (--count > 0);

    return r;
}

CALC_API char *CALC_CDECL strnupp(char *const dst, const char *const src, size_t count)
{
    if (!count)
        return (char *)src;

    char *r = strnget(dst, (const char *)NULL, count);

    do
        r[count - 1] = toupper(src[count - 1]);
    while (--count > 0);

    return r;
}

CALC_API char *CALC_CDECL strget(char *const dst, const char *const src)
{
    if (src)
        return strnget(dst, src, strlen(src));
    else
        return dimz(char, 1);
}

CALC_API char *CALC_CDECL strlow(char *const dst, const char *const src)
{
    if (src)
        return strnlow(dst, src, strlen(src));
    else
        return dst;
}

CALC_API char *CALC_CDECL strupp(char *const dst, const char *const src)
{
    if (src)
        return strnupp(dst, src, strlen(src));
    else
        return dst;
}

CALC_API bool_t CALC_CDECL streq(const char *const str1, const char *const str2)
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

CALC_API bool_t CALC_CDECL striq(const char *const str1, const char *const str2)
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

#pragma endregion

// +---- String Operations ---------------------------- END ----+

#pragma endregion

/* =------------------------------------------------------------= */

int main(int argc, char *argv[])
{
    char *c = strlow(NULL, "HEllO, worLD!");

    return EXIT_SUCCESS;
}
