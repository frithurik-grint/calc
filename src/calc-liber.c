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

char *strloc(size_t length)
{
	if (length > 0)
	{
		char *str;

		str = dim(char, length + 1);
		str[length] = '\0';

		return str;
	}
	else
	{
		return dimz(char, 1);
	}
}

bool_t streq(const char *const str1, const char *const str2)
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

bool_t strieq(const char *const str1, const char *const str2)
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

/* =---- Symbols Management ------------------------------------= */

#pragma region Symbols Management

static inline hash_t _get_hash_code(const char *str)
{
	if (!str || (*str <= 0x20))
		return HASH_INV; // intentionally returns an unmanageable value

	hash_t hash;

	for (hash = HASH_MIN; *str > 0x20; str++)
		hash += *str - 0x21;

	return hash;
}

hash_t gethash(const char *const str)
{
	return _get_hash_code((const char *)str);
}

// Symbol Table

symbkey_t *create_symbkey(const char *const name, unsigned int data, symbkey_t *const prev)
{
	symbkey_t *key = alloc(symbkey_t);

	key->name = name;
	key->data = data;
	key->refs = 0;

	if (prev)
	{
		if (prev->next)
			key->next = prev->next;
		else
			key->next = NULL;

		prev->next = key;
	}

	return key;
}

symbtab_t *create_symbtab(unsigned int size, symbtab_t *const prev)
{
	symbtab_t *tab = alloc(symbtab_t);

	tab->keys = dimz(symbkey_t, size);
	tab->size = size;
	tab->last = HASH_INV;
	tab->prev = prev;

	return tab;
}

#pragma endregion

/* =------------------------------------------------------------= */