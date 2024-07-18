#include "calc.h"

int main()
{
    char *lexm, line[BUFSIZ];
    lexer_t lex;

    lex.doub = create_doub(NULL, BUFSIZ);

    do
    {
        printf("> ");
        fgets(line, BUFSIZ, stdin);

        doub_puts(lex.doub, line);
        doub_retreat(lex.doub);

        tokcode_t c;

        do
        {
            c = lex_token(&lex, &lexm);
        } while (c != TOK_NULCH);

        printf("\n");
    } while (TRUE);
}