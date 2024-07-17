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

tokcode_t get_keyword_or_id(const char *const lexeme)
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

#pragma pop_macro("defkey")
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

static inline int expected(const char *const what, const char *const got)
{
    return printf("syntax error: expected %s, got '%s'\n", what, got);
}

static inline int notvalid(const char *const got)
{
    return printf("syntax error: '%s' is not a valid token\n", got);
}

tokcode_t gettok(doub_t *const src, char **const outlex)
{
    int l;

    if (outlex)
        *outlex = NULL;

    while (isspace(l = doub_topc(src)))
        src->buf++, src->pos++;

    switch (l)
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

        if ((l = doub_topc(src)) == '?')
            return src->fwd++, TOK_PUNCT_QUEST_QUEST;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_QUEST_EQUAL;
        else
            return TOK_PUNCT_QUEST;

    case '!':
        src->fwd++;

        if ((l = doub_topc(src)) == '!')
            return src->fwd++, TOK_PUNCT_EXCLM_EXCLM;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_EXCLM_EQUAL;
        else
            return TOK_PUNCT_EXCLM;

    case '&':
        src->fwd++;

        if ((l = doub_topc(src)) == '&')
            return src->fwd++, TOK_PUNCT_AMPER_AMPER;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_AMPER_EQUAL;
        else
            return TOK_PUNCT_AMPER;

    case '|':
        src->fwd++;
        
        if ((l = doub_topc(src)) == '|')
            return src->fwd++, TOK_PUNCT_PIPEE_PIPEE;
        else if (l == '=')
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

        if ((l = doub_topc(src)) == '<')
        {
            src->fwd++;

            if (doub_topc(src) == '=')
                return src->fwd++, TOK_PUNCT_LESST_LESST_EQUAL;
            else
                return TOK_PUNCT_LESST_LESST;
        }
        else if (l == '=')
        {
            return src->fwd++, TOK_PUNCT_LESST_EQUAL;
        }
        else if (l == '>')
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
        else if (l == '=')
        {
            return src->fwd++, TOK_PUNCT_GREAT_EQUAL;
        }
        else
        {
            return TOK_PUNCT_GREAT;
        }

    case '=':
        src->fwd++;
        
        if ((l = doub_topc(src)) == '=')
            return src->fwd++, TOK_PUNCT_EQUAL_EQUAL;
        else if (l == '>')
            return src->fwd++, TOK_PUNCT_THENN;
        else
            return TOK_PUNCT_EQUAL;

    case '+':
        src->fwd++;
        
        if ((l = doub_topc(src)) == '+')
            return src->fwd++, TOK_PUNCT_PLUSS_PLUSS;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_PLUSS_EQUAL;
        else
            return TOK_PUNCT_PLUSS;

    case '-':
        src->fwd++;

        if ((l = doub_topc(src)) == '-')
            return src->fwd++, TOK_PUNCT_MINUS_MINUS;
        else if (l == '=')
            return src->fwd++, TOK_PUNCT_MINUS_EQUAL;
        else if (l == '>')
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

        // Reserved Words

        // TODO...

        // Numerics

    case '0':
        do
            src->fwd++;
        while ((l = doub_topc(src)) == '0');

        if (((l = doub_topc(src)) == 'B') || (l == 'b'))
        {
            src->fwd++;

            if ((((l = doub_topc(src)) == '0') || (l == '1')))
            {
                doub_advance(src);

                do
                    src->fwd++;
                while ((((l = doub_topc(src)) == '0') || (l == '1')));
            }
            else
            {
                expected("binary digit", &l);
                doub_advance(src);
                
                return TOK_INVAL;
            }

            if (l == DECSEP)
                return 0;
            
            if (outlex)
                *outlex = doub_chop(src);

            return TOK_LITER_INTGR_BIN;
        }
        else if ((l == 'C') || (l == 'c'))
        {
            src->fwd++;

            if ((((l = doub_topc(src)) >= '0') && (l <= '7')))
            {
                doub_advance(src);

                do
                    src->fwd++;
                while ((((l = doub_topc(src)) >= '0') && (l <= '7')));
            }
            else
            {
                expected("octal digit", &l);
                doub_advance(src);

                return TOK_INVAL;
            }

            if (l == DECSEP)
                return 0;
            
            if (outlex)
                *outlex = doub_chop(src);
            
            return TOK_LITER_INTGR_OCT;
        }
        else if ((l == 'D') || (l == 'd') || isdigit(l))
        {
            src->fwd++;

            if ((((l = doub_topc(src)) >= '0') && (l <= '9')))
            {
                doub_advance(src);

    case '1':   case '2':   case '3':
    case '4':   case '5':   case '6':
    case '7':   case '8':   case '9':
                do
                    src->fwd++;
                while ((((l = doub_topc(src)) >= '0') && (l <= '9')));
            }
            else
            {
                expected("decimal digit", &l);
                doub_advance(src);

                return TOK_INVAL;
            }

            /*if (lookahead == DECSEP)
                return 0;*/

            if (outlex)
                *outlex = doub_chop(src);

            return TOK_LITER_INTGR_DEC;
        }
        else if ((l == 'X') || (l == 'x'))
        {
            src->fwd++;

            if (((((l = doub_topc(src)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F'))
                                                               || ((l >= 'a') && (l <= 'f'))))
            {
                doub_advance(src);

                do
                    src->fwd++;
                while (((((l = doub_topc(src)) >= '0') && (l <= '9')) || ((l >= 'A') && (l <= 'F'))
                                                                      || ((l >= 'a') && (l <= 'f'))));
            }
            else
            {
                expected("hexadecimal digit", &l);
                doub_advance(src);

                return TOK_INVAL;
            }

            if (l == DECSEP)
                return 0;

            if (outlex)
                *outlex = doub_chop(src);
            
            return TOK_LITER_INTGR_HEX;;
        }
        else
        {
            if (outlex)
                *outlex = "0";

            return TOK_LITER_INTGR_DEC;
        }

        // Others

    default:
        src->fwd++;

        notvalid(&l);
        doub_advance(src);

        return TOK_INVAL;
    }
}

const char *const tokcode_to_str(tokcode_t code)
{
    switch (code)
    {
    case TOK_INVAL:
        return "";

#pragma push_macro("defstr")

#ifndef defstr
#   define defstr(tok_name, tok_lexeme) case TOK_ ## tok_name: \
                                            return tok_lexeme;
#endif // defstr

#include "calc-parse.inc"

#ifdef defstr
#   undef defstr
#endif // defstr

#pragma pop_macro("defstr")

    default:
        return "uknown";
    }
}

#pragma endregion

#pragma endregion

/* =------------------------------------------------------------= */