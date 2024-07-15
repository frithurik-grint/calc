#include "calc-parse.h"

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Tokens

#pragma region Tokens

#pragma endregion

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

dbuf_t *create_dbuf(char *const buf, unsigned int length)
{
    dbuf_t *dbuf = alloc(dbuf_t);

    buf[length] = '\0';

    dbuf->buf = buf;
    dbuf->fwd = 0;
    dbuf->pos = 0;

    return dbuf;
}

int topbufc(const dbuf_t *const dbuf)
{
    int c;

    if (dbuf->buf[dbuf->fwd])
        c = dbuf->buf[dbuf->fwd];
    else
        c = EOF;

    return c;
}

int getbufc(dbuf_t *const dbuf)
{
    int c;

    if (dbuf->buf[dbuf->fwd])
        c = dbuf->buf[dbuf->fwd++];
    else
        c = EOF;

    return c;
}

int setbufc(dbuf_t *const dbuf, int c)
{
    if (dbuf->buf[dbuf->fwd])
        dbuf->buf[dbuf->fwd] = c;
    else
        c = EOF;

    return c;
}

int putbufc(dbuf_t *const dbuf, int c)
{
    if (dbuf->buf[dbuf->fwd])
        dbuf->buf[dbuf->fwd++] = c;
    else
        c = EOF;

    return c;
}

// Lexer

lexr_t *create_lexr(const char *const fname, dbuf_t *const dbuff)
{
    lexr_t *lexr = alloc(lexr_t);

    lexr->fname = fname;
    lexr->dbuff = dbuff;
    lexr->lnpos = 0;
    lexr->clpos = 0;

    return lexr;
}

static inline int _count_char(lexr_t *const lexr, int c)
{
    switch (c)
    {
    case '\n':
        lexr->lnpos++;
        lexr->clpos = 0;
        break;

    default:
        lexr->clpos++;
        break;
    }

    return c;
}

static inline int _skip_spaces(lexr_t *const lexr)
{
    dbuf_t *dbuf = lexr->dbuff;

    while (isspace(*dbuf->buf))
    {
        if (*dbuf->buf > 0)
        {
            dbuf->buf++;
            dbuf->pos++;
        }
        else
        {
            break;
        }

        _count_char(lexr, *dbuf->buf);
    }

    return *dbuf->buf;
}

int toplexc(lexr_t *const lexr)
{
    if (isspace(*lexr->dbuff->buf))
        _skip_spaces(lexr);

    return topbufc(lexr->dbuff);
}

int getlexc(lexr_t *const lexr)
{
    if (isspace(*lexr->dbuff->buf))
        _skip_spaces(lexr);

    return _count_char(lexr, getbufc(lexr->dbuff));
}

void advance(lexr_t *const lexr)
{
    dbuf_t *dbuf = lexr->dbuff;

    dbuf->buf += dbuf->fwd;
    dbuf->pos += dbuf->fwd;
    dbuf->fwd = 0;

    return;
}

char *accept(lexr_t *const lexr)
{
    char   *lexm;
    dbuf_t *dbuf;

    dbuf = lexr->dbuff;
    lexm = strdcpy(NULL, dbuf->buf, (size_t)dbuf->fwd);

    advance(lexr);

    return lexm;
}

char *retire(lexr_t *const lexr)
{
    return lexr->dbuff->fwd = 0, lexr->dbuff->buf;
}

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */