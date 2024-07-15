#include "calc-liber.h"

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

// +---- Internal

#pragma region Internal

static inline void *_safety_abort(
#ifdef CALC_DEBUG
	const char *const funcname
#endif // CALC_DEBUG
)
{
#ifdef CALC_DEBUG
	printf("error: function %s cannot allocate memory\n", funcname);
	exit(3);
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

// +---- Internal -- End

void *malloc_s(size_t size)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	);
}

void *calloc_s(size_t count, size_t size)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
	);
}

void *mallocz_s(size_t size)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	), size);
}

void *callocz_s(size_t count, size_t size)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
	), count * size);
}

#ifndef _get_aligned_size
#	define _get_aligned_size(size, alignment) ((size + alignment - 1) & ~(alignment - 1))
#endif // _get_aligned_size

void *malloca_s(size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(_get_aligned_size(size, alignment))
	);
}

void *calloca_s(size_t count, size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, _get_aligned_size(size, alignment))
	);
}

void *mallocaz_s(size_t size, size_t alignment)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size = _get_aligned_size(size, alignment))
	), size);
}

void *callocaz_s(size_t count, size_t size, size_t alignment)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size = _get_aligned_size(size, alignment))
	), count * size);
}

#ifdef _get_aligned_size
#	undef _get_aligned_size
#endif // undef _get_aligned_size

#pragma endregion

/* =---- Datatypes Management ----------------------------------= */

#pragma region Datatypes Management

// String

char *strdcpy(char *const dest, const char *const source, size_t length)
{
	char *buf;

	if (!dest)
		buf = dimz(char, length + 1);
	else
		buf = dest, buf[length] = '\0';

	return strncpy(buf, source, length);
}

#pragma endregion

/* =------------------------------------------------------------= */