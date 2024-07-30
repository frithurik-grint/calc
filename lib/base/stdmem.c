#include "calc/base/stdmem.h"

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

// +---- Internal (Memory Checks) ------------------------------+

#pragma region Internal (Memory Checks)

static inline void *_safety_abort(
#ifdef CALC_DEBUG
    const char *const funcname
#endif // CALC_DEBUG
)
{
#ifdef CALC_DEBUG
    errorf("fatal error: function %s cannot allocate memory\n", funcname);
    exit(EXIT_ABORTED);
#else
    raise(SIGSEGV);
    abort();
#endif // CALC_DEBUG

    return NULL;
}

static inline void *_safety_check(
#ifdef CALC_DEBUG
    const char *const funcname,
#endif // CALC_DEBUG
    void *block
)
{
    if (!block)
        return _safety_abort(
#ifdef CALC_DEBUG
            funcname
#endif // CALC_DEBUG
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

// +---- Internal (Memory Checks) ------- End ------------------+

_API void *_CDECL malloc_s(size_t size)
{
    return _safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        malloc(size)
    );
}

_API void *_CDECL calloc_s(size_t count, size_t size)
{
    return _safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        calloc(count, size)
    );
}

_API void *_CDECL mallocz_s(size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        malloc(size)
    ), size);
}

_API void *_CDECL callocz_s(size_t count, size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        calloc(count, size)
    ), count * size);
}

_API void *_CDECL malloca_s(size_t size, size_t alignment)
{
    return _safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        malloc(alignto(size, alignment))
    );
}

_API void *_CDECL calloca_s(size_t count, size_t size, size_t alignment)
{
    return _safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        calloc(count, alignto(size, alignment))
    );
}

_API void *_CDECL mallocaz_s(size_t size, size_t alignment)
{
    return size = alignto(size, alignment), _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        malloc(size)
    ), size);
}

_API void *_CDECL callocaz_s(size_t count, size_t size, size_t alignment)
{
    return size = alignto(size, alignment), _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        calloc(count, size)
    ), count * size);
}

// Strings

_API char *_CDECL stralloc(size_t length)
{
    return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
        __func__,
#endif // CALC_DEBUG
        malloc(length + 1)
    ), length + 1);
}

#pragma endregion

/* =------------------------------------------------------------= */
