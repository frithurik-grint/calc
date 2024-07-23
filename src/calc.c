#include "calc.h"

#include <stdint.h>

int main()
{
    char line[BUFSIZ];
    lexer_t *const lex = create_lexer(BUFSIZ, BUFSIZ);

    printf("TOK > ");
    fgets(line, BUFSIZ, stdin);
    dputs(lex->doub, line);

    ast_expr_t *root = parse_ast_expr_bnary(lex);
}