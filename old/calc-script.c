#ifndef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <assert.h>
#include <string.h>
#include <ctype.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#   include <direct.h>
#else
#   include <unistd.h>
#endif

#pragma warning(disable: 4996)

#ifndef _ENDLN
#   ifdef _WIN32
#       define _ENDLN "\r\n"
#   else
#       define _ENDLN "\n"
#   endif
#endif

#ifndef _PATH_SEPARATOR
#   ifdef _WIN32
#       define _PATH_SEPARATOR '\\'
#   else
#       define _PATH_SEPARATOR '/'
#   endif
#endif

#ifndef _ALT_PATH_SEPARATOR
#   ifdef _WIN32
#       define _ALT_PATH_SEPARATOR '/'
#   else
#       define _ALT_PATH_SEPARATOR '/'
#   endif
#endif

#ifndef _DRIV_SEPARATOR
#   define _DRIV_SEPARATOR ':'
#endif

#ifndef _EXTN_SEPARATOR
#   define _EXTN_SEPARATOR '.'
#endif

/* =---- Memory Management -------------------------------------= */

static inline void *malloc_s(size_t size);
static inline void *calloc_s(size_t count, size_t size);

#ifndef alloc
#   define alloc(type) (type *)malloc_s(sizeof(type))
#endif

#ifndef dim
#   define dim(type, count) (type *)calloc_s((count), sizeof(type))
#endif

static inline void *mallocz_s(size_t size);
static inline void *callocz_s(size_t count, size_t size);

#ifndef allocz
#   define allocz(type) (type *)mallocz_s(sizeof(type))
#endif

#ifndef dimz
#   define dimz(type, count) (type *)callocz_s((count), sizeof(type))
#endif

static inline void *malloca_s(size_t size, size_t alignment);
static inline void *calloca_s(size_t count, size_t size, size_t alignment);

#ifndef alloca
#   define alloca(type, alignment) (type *)mallocz_s(sizeof(type), (alignment))
#endif

#ifndef dima
#   define dima(type, count, alignment) (type *)callocz_s((count), sizeof(type), (alignment))
#endif

static inline char *strdupto(char *const dest, const char *const source, size_t length);

/* =---- Basic Datatypes ---------------------------------------= */

// bool_t

typedef unsigned char bool_t;

#ifndef TRUE
#   define TRUE ((bool_t)0x01)
#endif

#ifndef FALSE
#   define FALSE ((bool_t)0x00)
#endif

// byte_t

typedef unsigned char byte_t;

#ifndef BYTE_MIN
#   define BYTE_MIN ((byte_t)0x00)
#endif

#ifndef BYTE_MAX
#   define BYTE_MAX ((byte_t)0xFF)
#endif

/* =---- Command Line ------------------------------------------= */

// command line options

typedef enum e_optcode
{
    OPT_NONE = 0,
    OPT_FILE,

    OPT_HELP,
    OPT_VERS,
    OPT_ARGS,
    OPT_MAXS,
    OPT_STDI,
    OPT_STDO,

    OPT_FLAG_SHELL,
    OPT_FLAG_ADAPT,
} optcode_t;

typedef struct e_optn
{
    optcode_t         code;
    const char *const name;
    const char *const abbr;
    const char *const help;
} optn_t;

const optn_t *const getopt(const char *const text, bool_t abbr);

// command line interface

char *fgetdriv(char *const dest, const char *const filename, char **endptr);
char *fgetpath(char *const dest, const char *const filename, char **endptr);
char *fgetname(char *const dest, const char *const filename, char **endptr);
char *fgetbase(char *const dest, const char *const filename, char **endptr);
char *fgetextn(char *const dest, const char *const filename, char **endptr);

#ifndef CALC_ALIASAS
#   define CALC_ALIASAS "CALCScript Shell"
#endif

#ifndef CALC_VERSION
#   define CALC_VERSION "1.0.1-beta"
#endif

#ifndef CALC_LICENSE
#   define CALC_LICENSE "Copyright (c) 2024 Frithurik Grint"
#endif

int printh(const char *const execname);
int printv();

static inline int expected(const char *const expectation, const char *const got);

// environment

typedef struct s_stat
{
    // command line infos...
    int argc;
    int argn;

    char **argv;

    // executable file infos...
    char *exec_path;
    char *exec_name;

    char *cwd;

    // options...
    byte_t opt_argcount;
    byte_t opt_argindex;
    byte_t opt_maxstack;

    // flags...
    bool_t flag_adapt;
    bool_t flag_shell;
    bool_t flag_appnd;
    bool_t flag_exits;

    // actions...
    bool_t act_help;
    bool_t act_vers;
} stat_t;

stat_t *newstat(int argc, char **const argv);
stat_t *setstat(stat_t *const stat, optcode_t code);

/* =---- Main Code ---------------------------------------------= */

#ifndef CALC_HEADER
#   define CALC_HEADER CALC_ALIASAS " " CALC_VERSION " - " CALC_LICENSE _ENDLN \
                       "Type :help to print help message."              _ENDLN
#endif

int shell(stat_t *const stat);
// int repl();

int _main(int argc, char *argv[])
{
    int exitcode;

    if (argc > 0)
    {
        stat_t *stat = newstat(argc, argv);

        if (stat->act_help)
            printh(stat->exec_name);

        if (stat->act_vers)
            printv();

        if (stat->flag_shell)
            exitcode = shell(stat);
        else
            exitcode = EXIT_SUCCESS;
    }
    else
    {
        puts("error: invalid number of arguments");

        exitcode = EXIT_FAILURE;
    }

    return exitcode;
}

int shell(stat_t *const stat)
{
    puts(CALC_HEADER);

    char c, buf[BUFSIZ];

    stat->cwd = dimz(char, FILENAME_MAX);

    do
    {
        printf("%s> ", getcwd(stat->cwd, FILENAME_MAX));
        c = getchar();

        while (isspace(c))
            c = getchar();

        switch (c)
        {
        case ':':
            fgets(buf, BUFSIZ, stdin);
            system(buf);
            break;

        default:
            break;
        }
    } while (!stat->flag_exits);

    return EXIT_SUCCESS;
}

/* =---- Implementation ----------------------------------------= */

// memory management

#pragma region memory management implementation

static inline void *_safety_abort(
#ifdef _DEBUG
    const char *const funcname
#endif
)
{
#ifdef _DEBUG
    printf("error: function %s cannot allocate memory\n", funcname);
    exit(3);
#else
    printf("error: not enough memory.\n");
    abort();
#endif

    return NULL;
}

static inline void *_safety_check(
#ifdef _DEBUG
    const char *const funcname,
#endif
    void *block
)
{
    if (!block)
        return _safety_abort(
#ifdef _DEBUG
            funcname
#endif
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

static inline void *malloc_s(size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif
        malloc(size)
    );
}

static inline void *calloc_s(size_t count, size_t size)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif
        calloc(count, size)
    );
}

static inline void *mallocz_s(size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif
        malloc(size)
    ), size);
}

static inline void *callocz_s(size_t count, size_t size)
{
    return _zerofy_block(_safety_check(
#ifdef _DEBUG
        __func__,
#endif
        calloc(count, size)
    ), count * size);
}

#ifndef _get_aligned_size
#	define _get_aligned_size(size, alignment) ((size + alignment) & ~(alignment - 1))
#endif

static inline void *malloca_s(size_t size, size_t alignment)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif
        malloc(_get_aligned_size(size, alignment))
    );
}

static inline void *calloca_s(size_t count, size_t size, size_t alignment)
{
    return _safety_check(
#ifdef _DEBUG
        __func__,
#endif
        calloc(count, _get_aligned_size(size, alignment))
    );
}

#ifdef _get_aligned_size
#	undef _get_aligned_size
#endif

static inline char *strdupto(char *const dest, const char *const source, size_t length)
{
    char *buf;

    if (!dest)
        buf = dimz(char, length + 1);
    else
        buf = dest, buf[length] = '\0';

    return strncpy(buf, source, length);
}

#pragma endregion

// command line options

#pragma region command line options

#ifndef _ORDERED_OPTS
#   define _ORDERED_OPTS 0 // set to 1 only if arguments are in alphabetical order
#endif 

#ifndef defopt
#   define defopt(opt_code, opt_name, opt_abbr, opt_helptext)  { opt_code, opt_name, opt_abbr, opt_helptext },
#endif

static const optn_t _atab[] = {
    defopt(OPT_ARGS,       "arguments",        "a",               "set which commad line arguments pass to execution engine")
    defopt(OPT_FLAG_ADAPT, "flag-adapt",       "ad",              "enable stack auto growing")
    defopt(OPT_FLAG_SHELL, "flag-shell",       "sh",              "set execution mode to shell")
    defopt(OPT_HELP,       "help",             "h",               "print help message")
    defopt(OPT_MAXS,       "maxstack",         "ms",              "set max width of the evaluation stack")
    defopt(OPT_STDI,       "stdin",            NULL,              "redirect input stream to a file")
    defopt(OPT_STDO,       "stdout",           NULL,              "redirect output stream to a file")
    defopt(OPT_VERS,       "version",          "v",               "print current version")
    defopt(OPT_NONE,       NULL,               NULL,              NULL) // keep at the end
};

#ifdef defopt
#   undef defopt
#endif

const optn_t *const getopt(const char *const text, bool_t abbr)
#if _ORDERED_OPTS
{
    // TODO: binary search trough options
}
#else
{
    assert(text != NULL);

    optn_t *argtab = (optn_t *)_atab, *argptr = NULL;
    
    do
    {
        if ((!abbr) ? !stricmp(text, argtab->name)
                    : !stricmp(text, argtab->abbr))
            argptr = argtab;
        else
            ++argtab;
    } while ((argtab->code != OPT_NONE) && !argptr);

    return argptr;
}
#endif

#pragma endregion

// command line interface

#pragma region command line interface

char *fgetdriv(char *const dest, const char *const filename, char **endptr)
{
    assert(filename != NULL);

    size_t i, sep, len;

    len = strlen(filename);
    sep = 0;

    for (i = 0; i < len; i++)
    {
        if (filename[i] == _DRIV_SEPARATOR)
            sep = i + 1;
        else
            continue;
    }

    if (endptr)
        *endptr = (char *)(filename + sep);

    return strdupto(dest, filename, sep);
}

char *fgetpath(char *const dest, const char *const filename, char **endptr)
{
    assert(filename != NULL);
    
    size_t i, sep, len;

    len = strlen(filename);
    sep = 0;

    for (i = sep; i < len; i++)
    {
        if ((filename[i] == _PATH_SEPARATOR) || (filename[i] == _ALT_PATH_SEPARATOR))
            sep = i + 1;
        else
            continue;
    }

    if (endptr)
        *endptr = (char *)(filename + sep);

    return strdupto(dest, filename, sep);
}

char *fgetname(char *const dest, const char *const filename, char **endptr)
{
    assert(filename != NULL);

    size_t i, sep, len;

    len = strlen(filename);
    sep = 0;

    for (i = sep; i < len; i++)
    {
        if ((filename[i] == _PATH_SEPARATOR) || (filename[i] == _ALT_PATH_SEPARATOR))
            sep = i + 1;
        else
            continue;
    }

    if (endptr)
        *endptr = (char *)filename;

    return strdupto(dest, (filename + sep), (len - sep));
}

char *fgetbase(char *const dest, const char *const filename, char **endptr)
{
    assert(filename != NULL);

    size_t i, sep, dot, len;

    len = strlen(filename);
    sep = 0;

    for (i = sep; i < len; i++)
    {
        if ((filename[i] == _PATH_SEPARATOR) || (filename[i] == _ALT_PATH_SEPARATOR))
            sep = i + 1;
        else
            continue;
    }

    dot = len;

    for (i = sep; i < len; i++)
    {
        if (filename[i] == _EXTN_SEPARATOR)
            dot = i;
        else
            continue;
    }

    if (endptr)
        *endptr = (char *)filename;

    return strdupto(dest, (filename + sep), (dot - sep));
}

char *fgetextn(char *const dest, const char *const filename, char **endptr)
{
    assert(filename != NULL);

    size_t i, dot, len;

    len = strlen(filename);
    dot = 0;

    for (i = dot; i < len; i++)
    {
        if (filename[i] == _EXTN_SEPARATOR)
            dot = i + 1;
        else
            continue;
    }

    if (endptr)
        *endptr = (char *)filename;

    return strdupto(dest, (filename + dot), (len - dot));
}

static const char *const _htxt = {                                   _ENDLN
    "" CALC_ALIASAS " " CALC_VERSION " help message - " CALC_LICENSE _ENDLN _ENDLN
    "usage: %s [options...] [-c] infile(s)... [-a arguments...]"     _ENDLN
    "       %s [options...] -sh [arguments...]"                      _ENDLN _ENDLN
    "options:"                                                       _ENDLN
};

int printh(const char *const execname)
{
    int result = 0;

    result += printf(_htxt, execname, execname);

    optn_t *argtab;

    for (argtab = (optn_t *)_atab; argtab->code != OPT_NONE; argtab++)
    {
        if (!argtab->abbr)
            result += printf("  --%-16s       %-s" _ENDLN, argtab->name, argtab->help);
        else
            result += printf("  --%-16s -%-4s %-s" _ENDLN, argtab->name, argtab->abbr, argtab->help);
    }

    return result;
}

int printv()
{
    return puts(CALC_VERSION);
}

static inline int expected(const char *const expectation, const char *const got)
{
    return printf("error: expected %s (got '%s')" _ENDLN, expectation, got);
}

#pragma endregion

// environment

#pragma region environment

stat_t *newstat(int argc, char **const argv)
{
    stat_t *stat = allocz(stat_t);

    stat->argc = argc;
    stat->argv = argv;

    stat->exec_path = fgetpath(NULL, *stat->argv, NULL);
    stat->exec_name = fgetbase(NULL, *stat->argv, NULL);

    stat->flag_adapt = TRUE;

    if (argc > 1)
    {
        int opt;

        for (stat->argn = 1; stat->argn < argc; stat->argn++)
        {
            switch (*stat->argv[stat->argn])
            {
                bool_t abbr;

            case '-':
                ++(stat->argv[stat->argn]);

                if (*stat->argv[stat->argn] == '-')
                    abbr = FALSE, ++(stat->argv[stat->argn]);
                else
                    abbr = TRUE;

                opt = getopt(stat->argv[stat->argn], abbr)->code;

                break;

            default:
                opt = OPT_FILE;
                break;
            }

            if (opt != OPT_NONE)
                setstat(stat, opt);
            else
                continue;
        }
    }

    return stat;
}

stat_t *setstat(stat_t *const stat, optcode_t code)
{
    switch (code)
    {
        int tmp;

    case OPT_HELP:
        stat->act_help = TRUE;
        break;

    case OPT_VERS:
        stat->act_vers = TRUE;
        break;

    case OPT_ARGS:
        tmp = stat->argn;

        while (((tmp + 1) < stat->argc) && (*stat->argv[tmp + 1] != '-'))
            ++stat->opt_argcount, ++tmp;

        stat->opt_argcount = tmp - stat->argn;
        stat->opt_argindex = stat->argn;

        stat->argn = tmp;

        break;

    case OPT_MAXS:
        if (((stat->argn + 1) < stat->argc) && (*stat->argv[stat->argn + 1] != '-'))
        {
            char *arg;

            arg = stat->argv[stat->argn + 1];
            tmp = (int)strtoul(arg, NULL, 10);
                
            stat->argn++;

            if ((tmp > BYTE_MIN) && (tmp <= BYTE_MAX))
                stat->opt_maxstack = (byte_t)tmp;
            else
                expected("maxstack value", arg);
        }
        else
        {
            expected("maxstack value", stat->argv[stat->argn + 1]);
        }

        break;

    case OPT_STDI:
        if (((stat->argn + 1) < stat->argc) && (*stat->argv[stat->argn + 1] != '-'))
        {
            char *arg;
            FILE *fil;
            
            arg = stat->argv[stat->argn + 1];
            fil = freopen(arg, "r", stdin);

            stat->argn++;

            if (!fil)
                printf("error: cannot open '%s'" _ENDLN, arg);
        }
        else
        {
            expected("stream name", stat->argv[stat->argn + 1]);
        }

        break;

    case OPT_STDO:
        if (((stat->argn + 1) < stat->argc) && (*stat->argv[stat->argn + 1] != '-'))
        {
            char *arg;
            FILE *fil;

            arg = stat->argv[stat->argn + 1];
            fil = freopen(arg, (stat->flag_appnd ? "a+" : "w+"), stdout);

            stat->argn++;

            if (!fil)
                printf("error: cannot open '%s'" _ENDLN, arg);
        }
        else
        {
            expected("stream name", stat->argv[stat->argn + 1]);
        }

        break;

    case OPT_FLAG_SHELL:
        stat->flag_shell = TRUE;
        break;

    default:
        stat->act_help = TRUE;

        printf("error: uknown command '%s'" _ENDLN, stat->argv[stat->argn]);

        break;
    }

    return (stat_t *)stat;
}

#pragma endregion

/* =------------------------------------------------------------= */