#include "calc-parse.h"

#ifndef CALC_PARSE_C_
#define CALC_PARSE_C_

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

dbuf_t *create_dbuf(char *const buf, size_t length)
{
    dbuf_t *dbuf = alloc(dbuf_t);

    dbuf->buf = buf;
    dbuf->bgn = 0;
    dbuf->fwd = 0;
    dbuf->len = length;

    return dbuf;
}

int topbufc(const dbuf_t *const dbuf)
{
    int c;

    if (dbuf->buf && (dbuf->fwd < dbuf->len))
        c = dbuf->buf[dbuf->fwd];
    else
        c = EOF;

    return c;
}

int getbufc(dbuf_t *const dbuf)
{
    int c;

    if (dbuf->buf && (dbuf->fwd < dbuf->len))
        c = dbuf->buf[dbuf->fwd++];
    else
        c = EOF;

    return c;
}

int setbufc(dbuf_t *const dbuf, int c)
{
    if (dbuf->buf && (dbuf->fwd < dbuf->len))
        dbuf->buf[dbuf->fwd] = c;
    else
        c = EOF;

    return c;
}

int putbufc(dbuf_t *const dbuf, int c)
{
    if (dbuf->buf && (dbuf->fwd < dbuf->len))
        dbuf->buf[dbuf->fwd++] = c;
    else
        c = EOF;

    return c;
}

char *accept(dbuf_t *const dbuf)
{
    char *lexeme;

    lexeme = strdcpy(NULL, dbuf->buf + dbuf->bgn, (size_t)(dbuf->fwd - dbuf->bgn));
    dbuf->bgn = dbuf->fwd;

    return lexeme;
}

char *retire(dbuf_t *const dbuf)
{
    return dbuf->fwd = dbuf->bgn, dbuf->buf + dbuf->bgn;
}

void advance(dbuf_t *const dbuf)
{
    dbuf->bgn = dbuf->fwd;

    return;
}

void retreat(dbuf_t *const dbuf)
{
    dbuf->fwd = dbuf->bgn;

    return;
}

void reset(dbuf_t *const dbuf)
{
    dbuf->bgn = 0;
    dbuf->fwd = 0;

    return;
}

#pragma endregion

// +---- Tokens

#pragma region Tokens

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

#endif // CALC_PARSE_C_