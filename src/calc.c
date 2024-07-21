#include "calc.h"

#include <stdint.h>

uint64_t test_expr(ast_expr_t *const expr)
{
    static uint64_t stack[32];
    static uint32_t index = 0;

    switch (expr->kind)
    {
    case AST_EXPR_BINRY:
        test_expr(expr->node.binexpr->lhs);
        test_expr(expr->node.binexpr->rhs);

        switch (expr->node.binexpr->op)
        {
            uint64_t lhs, rhs;

        case AST_BINOP_ADD:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs + rhs;
            break;

        case AST_BINOP_SUB:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs - rhs;
            break;

        case AST_BINOP_MUL:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs * rhs;
            break;

        case AST_BINOP_DIV:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs / rhs;
            break;

        default:
            break;
        }

        break;

    case AST_EXPR_UNSIG:
        stack[index++] = expr->node.uint;
        break;
    }

    return stack[index - 1];
}

int main()
{
    char line[BUFSIZ];
    lexer_t *const lex = create_lexer(BUFSIZ, BUFSIZ);

    printf("TOK > ");
    fgets(line, BUFSIZ, stdin);
    dputs(lex->doub, line);

    ast_expr_t *root = parse_expr(lex);

    printfn("%d", test_expr(root));
}