#include "../src/calc-liber.h"

#pragma region System Dependent Libraries

#ifdef _WIN32

static SYSTEM_INFO _system_info = { 0 };

const int _getpagesiz()
{
	if (!_system_info.dwPageSize)
		GetSystemInfo(&_system_info);

	return (const int)_system_info.dwPageSize;
}

#endif // _WIN32

#pragma endregion

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

void *malloca_s(size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(alignto(size, alignment))
	);
}

void *calloca_s(size_t count, size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, alignto(size, alignment))
	);
}

void *mallocaz_s(size_t size, size_t alignment)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size = alignto(size, alignment))
	), size);
}

void *callocaz_s(size_t count, size_t size, size_t alignment)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size = alignto(size, alignment))
	), count * size);
}

// Strings

char *stralloc(size_t length)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(length + 1)
	), length + 1);
}

#pragma endregion

/* =---- Datatypes Management ----------------------------------= */

#pragma region Datatypes Management

// Strings

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
		buf = stralloc(length);
	else
		buf = dest, buf[length] = '\0';

	return strncpy(buf, source, length);
}

/* =---- Symbols Management ------------------------------------= */

#pragma region Symbols Management

// +---- Internal

#pragma region Internal

static inline hash_t _get_hashcode(const char *str)
{
	if (!str || (*str <= 0x20))
		return HASH_INV; // intentionally returns an unmanageable value

	hash_t hash;

	for (hash = HASH_MIN; *str > 0x20; str++)
		hash += *str - 0x21;

	return hash;
}

static inline hash_t _get_hashcode_mod(const char *str, unsigned int siz)
{
	return _get_hashcode(str) % siz;
}

#pragma endregion

// +---- Internal End

#pragma endregion

hash_t gethash(const char *const str)
{
	return _get_hashcode((const char *)str);
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
	else
	{
		key->next = NULL;
	}

	return key;
}

symbtab_t *create_symbtab(unsigned int size, symbtab_t *const prev)
{
	symbtab_t *tab = alloc(symbtab_t);

	tab->keys = dimz(symbkey_t, size);
	tab->size = size;
	tab->used = 0;
	tab->prev = prev;

	return tab;
}

bool_t symbtab_exists(symbtab_t *const tab, const char *const name)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		symbkey_t *s = tab->keys[hash];

		do
		{
			if (streq(name, s->name))
				return TRUE;
			else
				s = s->next;
		} while (s);
	}

	return FALSE;
}

symbkey_t *symbtab_add(symbtab_t *const tab, const char *const name, unsigned int attr)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		symbkey_t *s = tab->keys[hash], *p;

		do
		{
			if (streq(name, s->name))
				return s;
			else
				p = s, s = p->next;
		} while (s);

		return create_symbkey(name, attr, p);
	}
	else
	{
		return tab->used++, tab->keys[hash] = create_symbkey(name, attr, NULL);
	}
}

symbkey_t *symbtab_get(symbtab_t *const tab, const char *const name)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		symbkey_t *s = tab->keys[hash];

		do
		{
			if (streq(name, s->name))
				return s;
			else
				s = s->next;
		} while (s);
	}

	return NULL;
}

symbkey_t *symbtab_set(symbtab_t *const tab, const char *const name, unsigned int attr)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		symbkey_t *s = tab->keys[hash];

		do
		{
			if (streq(name, s->name))
				return s->data = attr, s;
			else
				s = s->next;
		} while (s);
	}

	return NULL;
}

symbtab_t *delete_symbtab(symbtab_t *const tab)
{
	unsigned int i;

	for (i = 0; tab->used && (i < tab->size); i++)
	{
		if (tab->keys[i])
		{
			symbkey_t *key = tab->keys[i], *tmp;

			do
			{
				tmp = key->next;

				free(key->name);
				free(key);

				key = tmp;
			} while (key);

			tab->used--;
		}
	}

	free(tab->keys);

	return (symbtab_t *)tab->prev;
}

#ifdef CALC_DEBUG

void symbtab_print(symbtab_t *const tab)
{
	unsigned int i, c, d;

	for (i = 0, c = tab->used; c && (i < tab->size); i++)
	{
		if (tab->keys[i])
		{
			symbkey_t *key = tab->keys[i];

			d = 0;

			do
			{
				printf("0x%04X-%02X -> 0x%04X ('%s')\n", i, d, key->data, key->name);

				key = key->next;
				d++;
			} while (key);

			c--;
		}
	}

	return;
}

#endif // CALC_DEBUG

#pragma endregion

/* =------------------------------------------------------------= */

#ifndef CALC_LIBER_C_
#	define CALC_LIBER_C_
#endif