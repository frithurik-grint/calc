#include "calc/base/stdmem.h"

/* =---- Memory Management -------------------------------------= */

// +---- Internal (Memory Checks) ------------------------------+

#pragma region Internal (Memory Checks)

static inline void *_safety_abort(
#ifdef _DEBUG
    const char *const funcname
#endif // _DEBUG
)
{
#ifdef _DEBUG
    fail("fatal error: function %s cannot allocate memory", funcname);
#else
    fail("fatal error: cannot allocate enough memory");
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

void *_CDECL checked_malloc(size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        malloc(size)
    );
}

void *_CDECL checked_calloc(size_t count, size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        calloc(count, size)
    );
}

void *_CDECL checked_mallocz(size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        malloc(size)
    ), size);
}

void *_CDECL checked_callocz(size_t count, size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        calloc(count, size)
    ), count * size);
}

// Aligned Allocators

#if !_CALC_BUILD_INLINES

void *_CDECL checked_aligned_malloc(size_t align, size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        malloc(alignto(size, align))
    );
}

void *_CDECL checked_aligned_calloc(size_t align, size_t count, size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        calloc(count, alignto(size, align))
    );
}

void *_CDECL checked_aligned_mallocz(size_t align, size_t size)
{
    return size = alignto(size, align), _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        malloc(size)
    ), size);
}

void *_CDECL checked_aligned_callocz(size_t align, size_t count, size_t size)
{
    return size = alignto(size, align), _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif // _DEBUG
        calloc(count, size)
    ), count * size);
}

#endif // _CALC_BUILD_INLINES

/* =------------------------------------------------------------= */
