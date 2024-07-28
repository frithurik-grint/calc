#include "calc-liber.h"

// +---- System Dependent Definitions --------------------------+

#pragma region System Dependent Definitions

#ifdef _WIN32

#include <windows.h>

_LOC unsigned int _CDECL _getpagsiz()
{
    static SYSTEM_INFO sys_info = { 0 };

    if (!sys_info.dwPageSize)
        GetSystemInfo(&sys_info);

    return sys_info.dwPageSize;
}

#endif // _WIN32

#pragma endregion

// +---- System Dependent Definitions -- End -------------------+

// +---- System Outputting Functions ---------------------------+

#pragma region System Outputting Functions

// +---- Internal (Output Functions) ---------------------------+

#pragma region Internal (Output Functions)

static inline int _fputln(FILE *const stream)
{
#ifdef _CALC_USE_CRLF
    return fputs("\r\n", stream);
#else
    return fputc('\n', stream), 1;
#endif // _CALC_USE_CRLF
}

static inline int _putln()
{
#ifdef _CALC_USE_CRLF
    return puts("\r\n");
#else
    return putchar('\n'), 1;
#endif // _CALC_USE_CRLF
}

static inline int _vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
    return vfprintf(stream, format, arglist) + _fputln(stream);
}

static inline int _vprintfn(const char *const format, va_list arglist)
{
    return vprintf(format, arglist) + _putln();
}

#pragma endregion

// +---- Internal (Output Functions) ---- End ------------------+

_API int _CDECL fputln(FILE *const stream)
{
    return _fputln(stream);
}

_API int _CDECL putln()
{
    return _putln();
}

// Print Functions

_API int _CDECL fprint(FILE *const stream, const char *const message)
{
    return fputs(message, stream);
}

_API int _CDECL fprintln(FILE *const stream, const char *const message)
{
    return fputs(message, stream) + _fputln(stream);
}

_API int _CDECL print(const char *const message)
{
    return puts(message);
}

_API int _CDECL println(const char *const message)
{
    return puts(message) + _putln();
}

_API int _CDECL vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
    return _vfprintfn(stream, format, arglist);
}

_API int _CDECL fprintfn(FILE *const stream, const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);
    
    result = _vfprintfn(stream, format, arglist);

    va_end(arglist);

    return result;
}

_API int _CDECL vprintfn(const char *const format, va_list arglist)
{
    return _vprintfn(format, arglist);
}

_API int _CDECL printfn(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = _vprintfn(format, arglist);

    va_end(arglist);

    return result;
}

// Error Print Functions

_API int _CDECL error(const char *const message)
{
    return fprint(stderr, message);
}

_API int _CDECL errorln(const char *const message)
{
    return fprintln(stderr, message);
}

_API int _CDECL verrorf(const char *const format, va_list arglist)
{
    return vfprintf(stderr, format, arglist);
}

_API int _CDECL verrorfn(const char *const format, va_list arglist)
{
    return vfprintfn(stderr, format, arglist);
}

_API int _CDECL errorf(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = vfprintf(stderr, format, arglist);

    va_end(arglist);

    return result;
}

_API int _CDECL errorfn(const char *const format, ...)
{
    int result;
    va_list arglist;

    va_start(arglist, format);

    result = vfprintfn(stderr, format, arglist);

    va_end(arglist);

    return result;
}

#pragma endregion

// +---- System Outputting Functions --- End -------------------+ 

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

/* =---- Exceptions Management ---------------------------------= */

#pragma region Exceptions Management

// +---- Internal (Exceptions Builder) -------------------------+

#pragma region Internal (Exceptions Builder)

static ex_t *_build_from_code(excode_t code)
{
    ex_t *ex = alloc(ex_t);

    switch (code)
    {
    default:
    case EX_UNHDL:
        ex->name = "UnhandledException";                // exception display name
        ex->mssg = "An exception has been thrown.";     // default exception message
        ex->kind = EX_KIND_INTRNL;                      // exception kind
        break;

    case EX_UNRCH:
        ex->name = "UnreachableException";
        ex->mssg = "Unreachable code has been reached.";
        ex->kind = EX_KIND_INTRNL;
        break;
    }

    return ex;
}

#pragma endregion

// +---- Internal (Exceptions Builder) -- End ------------------+

_API ex_t *_CDECL create_exception(char *const message, excode_t code)
{
    ex_t *ex = _build_from_code(code);

    if (message)
        ex->mssg = message;

    ex->file = NULL;
    ex->func = NULL;
    ex->line = 0;
    ex->prev = NULL;

    return ex;
}

_API ex_t *_CDECL create_exception_located(char *const message, excode_t code, char *const func, char *const file, unsigned int line)
{
    ex_t *ex = _build_from_code(code);

    if (message)
        ex->mssg = message;

    ex->file = file;
    ex->func = func;
    ex->line = line;
    ex->prev = NULL;

    return ex;
}

_API ex_t *_CDECL delete_exception(ex_t *const exception)
{
    ex_t *prev = exception->prev;

    free(exception);

    return prev;
}

// Exception Stack

_API ex_stack_t *_CDECL create_ex_stack()
{
    ex_stack_t *stack = alloc(ex_stack_t);

    stack->extcd = 0;
    stack->count = 0;
    stack->excpt = NULL;

    return stack;
}

_API void _CDECL delete_ex_stack(ex_stack_t *const stack)
{
    ex_t *ex = stack->excpt;

    while (ex)
        ex = delete_exception(ex);

    free(stack);

    return;
}

#ifndef _CALC_MINIMAL_BUILD

_API void _CDECL ex_stack_dump(FILE *const stream, ex_stack_t *const stack)
{
    if (stack->count == 0)
        return;

    ex_t *ex = stack->excpt;

    switch (stack->extcd)
    {
    case EXIT_SUCCESS:
        fprintfn(stream, "Process exited with %u exceptions:", stack->count);
        break;

    case EXIT_FAILURE:
        fprintfn(stream, "Process faulted with %u exceptions:", stack->count);
        break;

    case EXIT_ABORTED:
        fprintfn(stream, "Process aborted with %u exceptions:", stack->count);
        break;

    default:
        fprintfn(stream, "Process terminated (exit code: %u) with %u exceptions:", stack->extcd, stack->count);
        break;
    }

    while (ex)
        fprintfn(stream, "    %s: %s at %s in %s:%u", ex->name, ex->mssg, ex->func, ex->file, ex->line), ex = ex->prev;

    return;
}

#endif // _CALC_MINIMAL_BUILD

_API void _CDECL pushex(ex_stack_t *const stack, ex_t *const exception)
{
    exception->prev = stack->excpt;

    stack->excpt = exception;
    ++stack->count;

    return;
}

_API ex_t *_CDECL popex(ex_stack_t *const stack)
{
    ex_t *ex = stack->excpt;

    if (ex)
        stack->excpt = ex->prev, --stack->count;

    return ex;
}

_API ex_t *_CDECL peekex(ex_stack_t *const stack)
{
    return stack->excpt;
}

_API void _CDECL _ex_throw(ex_stack_t *const stack, const char *const file, const char *const func, unsigned int line, excode_t code, const char *const message)
{
    if (stack->extcd == EXIT_SUCCESS)
        stack->excpt = EXIT_FAILURE;

    pushex(stack, create_exception_located(message, code, func, file, line));

    return;
}

_API void _CDECL _ex_throwf(ex_stack_t *const stack, const char *const file, const char *const func, unsigned int line, excode_t code, const char *const format, ...)
{
    if (stack->extcd == EXIT_SUCCESS)
        stack->excpt = EXIT_FAILURE;

    va_list arglist;

    va_start(arglist, format);

    pushex(stack, create_exception_located(vformatn(format, arglist), code, func, file, line));

    va_end(arglist);

    return;
}

_API int _CDECL except(ex_stack_t *const stack, excode_t code, ex_callback_t callback)
{
    ex_t *ex;

    if (stack->excpt->code == code)
        return callback(popex(stack));
    else
        return -1;
}

#pragma endregion

/* =---- Data Management ---------------------------------------= */

#pragma region Data Management

// +---- Datatypes ---------------------------------------------+

#pragma region Datatypes

// String

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

// +---- Datatypes --------------------- End -------------------+

// +---- Datastructs (DSA) -------------------------------------+

#pragma region Datastructs

// Hash Bucket

_API hashbuc_t *_CDECL create_hashbuc(char *const name, hash_t hash, unsigned int data, hashbuc_t *const prev)
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

_API hashbuc_t *_CDECL delete_hashbuc(hashbuc_t *const bucket)
{
    hashbuc_t *next = bucket->next;

    free(bucket->name);
    free(bucket);

    return next;
}

// Hash Table

_API hashtab_t *_CDECL create_hashtab(unsigned int size, hashfnc_t func, hashtab_t *const prev)
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

_API hashtab_t *_CDECL delete_hashtab(hashtab_t *const tab)
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

_API hashbuc_t *_CDECL hashtab_add(hashtab_t *const tab, char *const key)
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

_API hashbuc_t *_CDECL hashtab_get(hashtab_t *const tab, char *const key)
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

_API hashbuc_t *_CDECL hashtab_set(hashtab_t *const tab, char *const key, unsigned int data)
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

_API bool_t _CDECL hashtab_contains(hashtab_t *const tab, char *const key)
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

_API hashbuc_t *_CDECL hashtab_remove(hashtab_t *const tab, char *const key)
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

_API hashbuc_t *_CDECL hashtab_delete(hashtab_t *const tab, char *const key)
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

_API void _CDECL hashtab_dump(FILE *const stream, hashtab_t *const tab)
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
                ++d;
            } while (buck);

            --c;
        }
    }

    return;
}

#endif // _CALC_MINIMAL_BUILD

#pragma endregion

// +---- Datastructs ------------------- End -------------------+

#pragma endregion

/* =------------------------------------------------------------= */

#ifndef CALC_LIBER_C_
#   define CALC_LIBER_C_
#endif // CALC_LIBER_C_
