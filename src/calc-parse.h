#pragma once

/* calc-parse.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_PARSE_H_
#define CALC_PARSE_H_ 1

#include "calc-liber.h"

#ifdef __cplusplus
#   include <cstdio>

namespace calc
{
#else
#   include <stdio.h>
#endif // __cplusplus

CALC_C_HEADER_BEGIN

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

typedef struct _calc_double_buffer
{
    char        *buf;
    unsigned int bgn;
    unsigned int fwd;
    size_t       len;
} dbuf_t;

dbuf_t *create_dbuf(char *const buf, size_t length);

int topbufc(const dbuf_t *const dbuf);
int getbufc(dbuf_t *const dbuf);
int setbufc(dbuf_t *const dbuf, int c);
int putbufc(dbuf_t *const dbuf, int c);

char *accept(dbuf_t *const dbuf);
char *retire(dbuf_t *const dbuf);

void advance(dbuf_t *const dbuf);
void retreat(dbuf_t *const dbuf);

void reset(dbuf_t *const dbuf);

#pragma endregion

// +---- Tokens

#pragma region Tokens

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CALC_PARSE_H_