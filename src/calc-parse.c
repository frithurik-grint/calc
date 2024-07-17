#include "calc-parse.h"

/* =---- Lexical Analyzer --------------------------------------= */

#pragma region Lexical Analyzer

// +---- Source Stream

#pragma region Source Stream

// Double Buffer

doub_t *create_doub(char *const buffer, unsigned int length)
{
    doub_t *buf = alloc(doub_t);

    if (!length)
        length = BUFSIZ;

    buf->buf = (!buffer ? strloc(length) : buffer);
    buf->pos = 0;
    buf->fwd = 0;
    buf->len = length;

    return buf;
}

int doub_topc(const doub_t *const buf)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd];
#else
        return buf->buf[buf->pos + buf->fwd];
#endif
    else
        return EOF;
}

int doub_getc(doub_t *const buf)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd++];
#else
        return buf->buf[buf->pos + buf->fwd++];
#endif
    else
        return EOF;
}

int doub_setc(doub_t *const buf, int c)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd] = c;
#else
        return buf->buf[buf->pos + buf->fwd] = c;
#endif
    else
        return EOF;
}

int doub_putc(doub_t *const buf, int c)
{
    if ((buf->pos + buf->fwd) < buf->len)
#ifdef CALC_DEBUG
        return buf->buf[buf->fwd++] = c;
#else
        return buf->buf[buf->pos + buf->fwd++] = c;
#endif
    else
        return EOF;
}

char *doub_gets(char *const dest, doub_t *const buf)
{
    int i = 0, c;

    do
    {
        c = doub_getc(buf);

        if (c == EOF)
            break;
        else
            dest[i++] = c;
    } while ((c != '\n') && (c != '\0'));

    dest[i] = '\0';

    return dest;
}

char *doub_puts(doub_t *const buf, char *const str)
{
    int i = 0, c;

    do
        c = doub_putc(buf, str[i++]);
    while ((c != EOF) && str[i]);

    doub_putc(buf, '\0');

    return str;
}

char *doub_chop(doub_t *const buf)
{
#ifdef CALC_DEBUG
    return strdcpy(NULL, buf->buf, buf->fwd);
#else
    return strdcpy(NULL, buf->buf + buf->pos, buf->fwd);
#endif
}

void doub_advance(doub_t *const buf)
{
    if ((buf->pos + buf->fwd) < buf->len)
    {
#ifdef CALC_DEBUG
        buf->buf += buf->fwd;
#endif
        buf->pos += buf->fwd;
        buf->fwd = 0;
    }
    else
    {
#ifdef CALC_DEBUG
        buf->buf += (buf->pos + buf->fwd) - buf->len;
#endif
        buf->pos += (buf->pos + buf->fwd) - buf->len;
        buf->fwd = 0;
    }

    return;
}

void doub_retreat(doub_t *const buf)
{
    buf->fwd = 0;
    return;
}

void doub_rewind(doub_t *const buf)
{
#ifdef CALC_DEBUG
    buf->buf -= buf->pos;
#endif
    buf->pos = 0;
    buf->fwd = 0;

    return;
}

char *doub_getbuf(doub_t *const buf)
{
#ifdef CALC_DEBUG
    return buf->buf;
#else
    return buf->buf + buf->pos;
#endif
}

#pragma endregion

// +---- Tokens

#pragma region Tokens

tokcode_t gettok_kword_or_id(const char *const lexeme)
{
    static const tok_t const keystab[] = {
#pragma push_macro("defkey")

#ifndef defkey
#   define defkey(tok_name, tok_lexeme) { .code = TOK_KWORD_ ## tok_name, .lexm = tok_lexeme },
#endif

#include "calc-parse.inc"

#ifdef defkey
#   undef defkey
#endif

#pragma pop_macro("deftok")
    };

    static const int const count = sizeof(keystab) / sizeof(*keystab);

#if CALC_SORTED_KEYWORDS    // if keywords are soreted, it does a binary search
    int hig = 0, mid, low = count - 1;

    do
    {
        mid = (hig + low) >> 1;

        switch (numcmp(*lexeme, *keystab[mid].lexm))
        {
        case +1:
            hig = mid + 1;
            break;

        case -1:
            low = mid - 1;
            break;

        default:
            if (streq(lexeme, keystab[mid].lexm))
                return keystab[mid].code;
            else
                hig = mid + 1;

            break;
        }
    } while (hig <= low);

    return TOK_IDENT;
#else                       // if keywords aren't sorted, it does a linear search
    int i;

    for (i = 0; i < count; i++)
    {
        if (streq(lexeme, keystab[i].lexm))
            return keystab[i].code;
        else
            continue;
    }

    return TOK_IDENT;
#endif
}

#ifndef _gettok_num
#   define _gettok_num(src, outlex, l, cond, flt, out)  do                                          \
                                                            src->fwd++;                             \
                                                        while ((cond));                             \
                                                                                                    \
                                                        if (l == DECSEP)                            \
                                                            return 0;                               \
                                                                                                    \
                                                        if (outlex)                                 \
                                                            *outlex = doub_chop(src);               \
                                                                                                    \
                                                        return out;
#endif // _gettok_num

#ifndef _gettok_num_bin
#   define _gettok_num_bin(src, outlex, l) _gettok_num(src, outlex, l, ((l = doub_topc(src)) == '0') && (l == '1'), 0, TOK_LITER_INTGR_BIN)
#endif // _gettok_num_bin

#ifndef _gettok_num_oct
#   define _gettok_num_oct(src, outlex, l) _gettok_num(src, outlex, l, ((l = doub_topc(src)) >= '0') && (l <= '7'), 0, TOK_LITER_INTGR_OCT)
#endif // _gettok_num_oct

#ifndef _gettok_num_dec
#   define _gettok_num_dec(src, outlex, l) _gettok_num(src, outlex, l, ((l = doub_topc(src)) >= '0') && (l <= '9'), 0, TOK_LITER_INTGR_DEC)
#endif // _gettok_num_dec

#ifndef _gettok_num_hex
#   define _gettok_num_hex(src, outlex, l) _gettok_num(src, outlex, l, (((l = doub_topc(src)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F'))  \
                                                                                                                     || ((l >= 'a') && (l <= 'f')), 0, TOK_LITER_INTGR_HEX)
#endif // _gettok_num_hex

tokcode_t gettok(doub_t *const src, char **const outlex)
{
    int lookahead;

    if (outlex)
        *outlex = NULL;

    while (isspace(lookahead = doub_topc(src)))
        src->fwd++;

    switch (lookahead)
    {
        // Trivia

    case EOF:
        return TOK_ENDOF;

    case NUL:
        return TOK_ENDOS;

        // Brackets

    case '(':
        return src->fwd++, TOK_PUNCT_LROUN;

    case ')':
        return src->fwd++, TOK_PUNCT_RROUN;

    case '[':
        return src->fwd++, TOK_PUNCT_LSQRD;

    case ']':
        return src->fwd++, TOK_PUNCT_RSQRD;

    case '{':
        return src->fwd++, TOK_PUNCT_LCURL;

    case '}':
        return src->fwd++, TOK_PUNCT_RCURL;

        // Punctuators

    case '~':
        return src->fwd++, TOK_PUNCT_TILDE;

    case '?':
        src->fwd++;

        if ((lookahead = doub_topc(src)) == '?')
            return src->fwd++, TOK_PUNCT_QUEST_QUEST;
        else if (lookahead == '=')
            return src->fwd++, TOK_PUNCT_QUEST_EQUAL;
        else
            return TOK_PUNCT_QUEST;

    case '!':
        src->fwd++;

        if ((lookahead = doub_topc(src)) == '!')
            return src->fwd++, TOK_PUNCT_EXCLM_EXCLM;
        else if (lookahead == '=')
            return src->fwd++, TOK_PUNCT_EXCLM_EQUAL;
        else
            return TOK_PUNCT_EXCLM;

    case '&':
        src->fwd++;

        if ((lookahead = doub_topc(src)) == '&')
            return src->fwd++, TOK_PUNCT_AMPER_AMPER;
        else if (lookahead == '=')
            return src->fwd++, TOK_PUNCT_AMPER_EQUAL;
        else
            return TOK_PUNCT_AMPER;

    case '|':
        src->fwd++;
        
        if ((lookahead = doub_topc(src)) == '|')
            return src->fwd++, TOK_PUNCT_PIPEE_PIPEE;
        else if (lookahead == '=')
            return src->fwd++, TOK_PUNCT_PIPEE_EQUAL;
        else
            return TOK_PUNCT_PIPEE;

    case '^':
        src->fwd++;
        
        if (doub_topc(src) == '=')
            return src->fwd++, TOK_PUNCT_CARET_EQUAL;
        else
            return TOK_PUNCT_CARET;

    case '<':
        src->fwd++;

        if ((lookahead = doub_topc(src)) == '<')
        {
            src->fwd++;

            if (doub_topc(src) == '=')
                return src->fwd++, TOK_PUNCT_LESST_LESST_EQUAL;
            else
                return TOK_PUNCT_LESST_LESST;
        }
        else if (lookahead == '=')
        {
            return src->fwd++, TOK_PUNCT_LESST_EQUAL;
        }
        else if (lookahead == '>')
        {
            return src->fwd++, TOK_PUNCT_EQVLN;
        }
        else
        {
            return TOK_PUNCT_LESST;
        }

    case '>':
        src->fwd++;

        if (doub_topc(src) == '>')
        {
            src->fwd++;

            if (doub_topc(src) == '=')
                return src->fwd++, TOK_PUNCT_GREAT_GREAT_EQUAL;
            else
                return TOK_PUNCT_GREAT_GREAT;
        }
        else if (lookahead == '=')
        {
            return src->fwd++, TOK_PUNCT_GREAT_EQUAL;
        }
        else
        {
            return TOK_PUNCT_GREAT;
        }

    case '=':
        src->fwd++;
        
        if ((lookahead = doub_topc(src)) == '=')
            return src->fwd++, TOK_PUNCT_EQUAL_EQUAL;
        else if (lookahead == '>')
            return src->fwd++, TOK_PUNCT_THENN;
        else
            return TOK_PUNCT_EQUAL;

    case '+':
        src->fwd++;
        
        if ((lookahead = doub_topc(src)) == '+')
            return src->fwd++, TOK_PUNCT_PLUSS_PLUSS;
        else if (lookahead == '=')
            return src->fwd++, TOK_PUNCT_PLUSS_EQUAL;
        else
            return TOK_PUNCT_PLUSS;

    case '-':
        src->fwd++;

        if ((lookahead = doub_topc(src)) == '-')
            return src->fwd++, TOK_PUNCT_MINUS_MINUS;
        else if (lookahead == '=')
            return src->fwd++, TOK_PUNCT_MINUS_EQUAL;
        else if (lookahead == '>')
            return src->fwd++, TOK_PUNCT_ARROW;
        else
            return TOK_PUNCT_MINUS;

    case '*':
        src->fwd++;
        
        if (doub_topc(src) == '=')
            return src->fwd++, TOK_PUNCT_STARR_EQUAL;
        else
            return TOK_PUNCT_STARR;

    case '/':
        src->fwd++;
        
        if (doub_topc(src) == '=')
            return src->fwd++, TOK_PUNCT_SLASH_EQUAL;
        else
            return TOK_PUNCT_SLASH;

    case '%':
        src->fwd++;
        
        if (doub_topc(src) == '=')
            return src->fwd++, TOK_PUNCT_PERCN_EQUAL;
        else
            return TOK_PUNCT_PERCN;

    case '#':
        return src->fwd++, TOK_PUNCT_SHARP;

    case '@':
        return src->fwd++, TOK_PUNCT_ATSIG;

    case ',':
        return src->fwd++, TOK_PUNCT_COMMA;

    case '.':
        src->fwd++;

        if (doub_topc(src) == '.')
        {
            src->fwd++;

            if (doub_topc(src) == '.')
                return src->fwd++, TOK_PUNCT_ELLIP;
            else
                return TOK_PUNCT_POINT_POINT;
        }
        else
        {
            return TOK_PUNCT_POINT;
        }

    case ':':
        src->fwd++;

        if (doub_topc(src) == ':')
            return src->fwd++, TOK_PUNCT_COLON_COLON;
        else
            return TOK_PUNCT_COLON;

    case ';':
        return src->fwd++, TOK_PUNCT_SEMIC;

    case '\\':
        return src->fwd++, TOK_PUNCT_BACKS;

    case '`':
        return src->fwd++, TOK_PUNCT_BACKT;

        // Numerics

    case '1':   case '2':   case '3':
    case '4':   case '5':   case '6':
    case '7':   case '8':   case '9':
        do
            src->fwd++;
        while ((((lookahead = doub_topc(src)) >= '0') && (lookahead <= '9')));

        /*if (lookahead == DECSEP)
            return 0;*/

        if (outlex)
            *outlex = doub_chop(src);

        return TOK_LITER_INTGR_DEC;

        // Others

    default:
        break;
    }
}

tok_t *create_token(tokcode_t const code, const char *const lexeme)
{
    tok_t *token = alloc(tok_t);

    token->lexm = lexeme;
    token->code = code;

    return token;
}

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */