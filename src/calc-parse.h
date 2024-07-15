#pragma once

/* calc-parse.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_PARSE_H_
#define CALC_PARSE_H_ 1

#include "calc-liber.h"

#ifdef __cplusplus
#   include <cctype>
#   include <cstdio>

namespace calc
{
#else
#   include <ctype.h>
#   include <stdio.h>
#endif // __cplusplus

CALC_C_HEADER_BEGIN

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Tokens

#pragma region Tokens

#pragma endregion

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

typedef struct _calc_double_buffer
{
    char        *buf;
    unsigned int fwd;
    unsigned int pos;
} dbuf_t;

dbuf_t *create_dbuf(char *const buf, unsigned int length);

int topbufc(const dbuf_t *const dbuf);
int getbufc(dbuf_t *const dbuf);
int setbufc(dbuf_t *const dbuf, int c);
int putbufc(dbuf_t *const dbuf, int c);

// Lexer

typedef struct _calc_lexer
{
    char        *fname;
    dbuf_t      *dbuff;
    unsigned int lnpos;
    unsigned int clpos;
} lexr_t;

lexr_t *create_lexr(const char *const fname, dbuf_t *const dbuff);

int toplexc(lexr_t *const lexr);
int getlexc(lexr_t *const lexr);

void advance(lexr_t *const lexr);

char *accept(lexr_t *const lexr);
char *retire(lexr_t *const lexr);

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CALC_PARSE_H_