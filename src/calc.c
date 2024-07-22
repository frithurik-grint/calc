#include "calc.h"

#include <stdint.h>

uint64_t test_expr(ast_expr_t *const expr)
{
    static int64_t stack[32];
    static int32_t index = 0;

    switch (expr->kind)
    {
    case AST_EXPR_UNARY:
        test_expr(expr->data.unary->val);

        switch (expr->data.unary->op)
        {
            int64_t val;

        case OP_UNARY_POS:
            val = stack[--index];
            stack[index++] = +val;
            break;

        case OP_UNARY_NEG:
            val = stack[--index];
            stack[index++] = -val;
            break;

        default:
            break;
        }

        break;

    case AST_EXPR_BNARY:
        test_expr(expr->data.bnary->lhs);
        test_expr(expr->data.bnary->rhs);

        switch (expr->data.bnary->op)
        {
            int64_t lhs, rhs;

        case OP_BNARY_ADD:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs + rhs;
            break;

        case OP_BNARY_SUB:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs - rhs;
            break;

        case OP_BNARY_MUL:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs * rhs;
            break;

        case OP_BNARY_DIV:
            rhs = stack[--index];
            lhs = stack[--index];

            stack[index++] = lhs / rhs;
            break;

        default:
            break;
        }

        break;

    case AST_EXPR_UNSIG:
        stack[index++] = (int64_t)expr->data.unsig;
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

    ast_expr_t *root = parse_ast_expr_bnary(lex);

    printfn("%d", test_expr(root));
}