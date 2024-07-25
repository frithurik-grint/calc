#include "calc-liber.h"

// +---- System Dependent Libraries

#pragma region System Dependent Libraries

#ifdef _WIN32

#include <windows.h>

static SYSTEM_INFO _system_info = { 0 };

const int _getpagesiz()
{
    if (!_system_info.dwPageSize)
        GetSystemInfo(&_system_info);

    return (const int)_system_info.dwPageSize;
}

#endif // _WIN32

// +---- System Dependent Libraries -- End

// +---- System Output Functions

#pragma region System Output Functions

// +---- Internal

static inline int _fputln(FILE *const stream)
#ifdef _CALC_USE_CRLF
{
	return fputs("\r\n", stream);
}
#else
{
	return fputc('\n', stream), 1;
}
#endif // _WIN32

static inline int _putln()
#ifdef _CALC_USE_CRLF
{
	return puts("\r\n");
}
#else
{
	return putchar('\n'), 1;
}
#endif // _WIN32

static inline int _vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
	return vfprintf(stream, format, arglist) + _fputln(stream);
}

static inline int _vprintfn(const char *const format, va_list arglist)
{
	return vprintf(format, arglist) + _putln();
}

#pragma endregion

// +---- Internal -- End

int fputln(FILE *const stream)
{
	return _fputln(stream);
}

int putln()
{
	return _putln();
}

int fprint(FILE *const stream, const char *const message)
{
	return fputs(message, stream);
}

int print(const char *const message)
{
	return puts(message);
}

int fprintln(FILE *const stream, const char *const message)
{
	return fputs(message, stream) + _fputln(stream);
}

int println(const char *const message)
{
	return puts(message) + _putln();
}

int vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
	return _vfprintfn(stream, format, arglist);
}

int fprintfn(FILE *const stream, const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = _vfprintfn(stream, format, arglist);
	va_end(arglist);

	return result;
}

int vprintfn(const char *const format, va_list arglist)
{
	return _vprintfn(format, arglist);
}

int printfn(const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = _vprintfn(format, arglist);
	va_end(arglist);

	return result;
}

#pragma endregion

// +---- Output Functions -- End

#pragma endregion

/* =---- Exceptions Management ---------------------------------= */

#pragma region Exceptions Management

int error(const char *const message)
{
	return fprint(stderr, message);
}

int errorln(const char *const message)
{
	return fprintln(stderr, message);
}

int verrorf(const char *const format, va_list arglist)
{
	return vfprintf(stderr, format, arglist);
}

int verrorfn(const char *const format, va_list arglist)
{
	return _vfprintfn(stderr, format, arglist);
}

int errorf(const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = verrorf(format, arglist);
	va_end(arglist);

	return result;
}

int errorfn(const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = verrorfn(format, arglist);
	va_end(arglist);

	return result;
}

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
	return size = alignto(size, alignment), _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	), size);
}

void *callocaz_s(size_t count, size_t size, size_t alignment)
{
	return size = alignto(size, alignment), _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
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

/* =---- Data Management ---------------------------------------= */

#pragma region Data Management

// +---- Datatypes

#pragma region Datatypes

// String

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

char *strntolower(char *const dest, const char *const source, size_t length)
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

char *strtolower(char *const dest, const char *const source)
{
	return strntolower(dest, source, strlen(source));
}

char *strntoupper(char *const dest, const char *const source, size_t length)
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

char *strtoupper(char *const dest, const char *const source)
{
	return strntoupper(dest, source, strlen(source));
}

char *strndcpy(char *const dest, const char *const source, size_t length)
{
	char *buf;

	if (!dest)
		buf = stralloc(length);
	else
		buf = dest, buf[length] = '\0';

	return strncpy(buf, source, length);
}

char *strdcpy(char *const dest, const char *const source)
{
	return strndcpy(dest, source, strlen(source));
}

char *unesc(char *const dest, int c)
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

char *vformatto(char *const dest, const char *const format, va_list arglist)
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

char *formatto(char *const dest, const char *const format, ...)
{
	char *result;
	va_list arglist;

	va_start(arglist, format);
	result = vformatto(dest, format, arglist);
	va_end(arglist);

	return result;
}

char *vformat(const char *const format, va_list arglist)
{
	return vformatto(NULL, format, arglist);
}

char *format(const char *const format, ...)
{
	char *result;
	va_list arglist;

	va_start(arglist, format);
	result = vformat(format, arglist);
	va_end(arglist);

	return result;
}

#pragma endregion

// +---- Datatypes -- End

// +---- Datastructs (DSA)

#pragma region Datastructs

// Hash Table

hashbuc_t *create_hashbuc(char *const name, hash_t hash, unsigned int data, hashbuc_t *const prev)
{
    hashbuc_t *bucket = alloc(hashbuc_t);

    bucket->name = name;
    bucket->hash = hash;
    bucket->data = data;

    if (prev)
        bucket->next = prev->next, prev->next = bucket;
    else
        bucket->next = NULL;

    return bucket;
}

hashbuc_t *delete_hashbuc(hashbuc_t *const bucket)
{
    hashbuc_t *next = bucket->next;

    free(bucket->name);
    free(bucket);

    return next;
}

hashtab_t *create_hashtab(unsigned int size, hashfnc_t func, hashtab_t *const prev)
{
    hashtab_t *tab = alloc(hashtab_t);

    if (!size)
        size = CALC_HASHTAB_BUCKSNUM;

    tab->buck = dimz(hashbuc_t *, size);
	tab->func = func;
    tab->size = size;
    tab->used = 0;
    tab->prev = prev;

    return tab;
}

hashtab_t *delete_hashtab(hashtab_t *const tab)
{
    unsigned int i;

    for (i = 0; tab->used && (i < tab->size); i++)
    {
        hashbuc_t *bucket = tab->buck[i];

        do
            bucket = delete_hashbuc(bucket);
        while (bucket);

        --tab->used;
    }

    hashtab_t *prev = tab->prev;

    free(tab->buck);
    free(tab);

    return prev;
}

hashbuc_t *hashtab_add(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashbuc_t *prev;

    if (tab->buck[hash])
    {
        hashbuc_t *buck = tab->buck[hash];

        do
        {
            if (streq(key, buck->name))
                return buck;
            else
                prev = buck, buck = prev->next;
        } while (buck);
    }
    else
    {
        prev = NULL;
    }

    return tab->used++, tab->buck[hash] = create_hashbuc(key, hash, 0, prev);
}

hashbuc_t *hashtab_get(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return buck;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);
    
    return NULL;
}

hashbuc_t *hashtab_set(hashtab_t *const tab, char *const key, unsigned int data)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return buck->data = data, buck;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);
    
    return NULL;
}

bool_t hashtab_contains(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return TRUE;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);
    
    return FALSE;
}

hashbuc_t *hashtab_remove(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return tbp->buck[hash] = buck->next, buck;
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);
    
    return NULL;
}

hashbuc_t *hashtab_delete(hashtab_t *const tab, char *const key)
{
    hash_t hash = tab->func(key) % tab->size;
    hashtab_t *tbp = tab;

    do
    {
        if (tbp->buck[hash])
        {
            hashbuc_t *buck = tbp->buck[hash];

            do
            {
                if (streq(key, buck->name))
                    return tbp->buck[hash] = delete_hashbuc(buck);
                else
                    buck = buck->next;
            } while (buck);
        }

        tbp = tbp->prev;
    } while (tbp);
    
    return NULL;
}

#ifndef _CALC_MINIMAL_BUILD

void hashtab_dump(FILE *const stream, hashtab_t *const tab)
{
    unsigned int i, c, d;

	for (i = 0, c = tab->used; c && (i < tab->size); i++)
	{
		if (tab->buck[i])
		{
			hashbuc_t *buck = tab->buck[i];

			d = 0;

			do
			{
				printf("0x%04X-%02X -> 0x%04X ('%s')\n", i, d, buck->data, buck->name);

				buck = buck->next;
				d++;
			} while (buck);

			c--;
		}
	}

	return;
}

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Datastructs -- End

#pragma endregion

/* =------------------------------------------------------------= */

#ifndef CALC_LIBER_C_
#	define CALC_LIBER_C_
#endif
