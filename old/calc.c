#include "calc.h"

#include <stdint.h>

static int64_t stack[32];
static int32_t index = 0;

static void test(ast_expr_t *const expr, uint32_t depth)
{
    int64_t val, lhs, rhs;

    switch (expr->kind)
    {
    case AST_EXPR_UNSIG:
        stack[index++] = expr->data.signd;
        break;

    case AST_EXPR_UNARY:
        test(expr->data.unary->val, depth + 1);

        val = stack[--index];

        switch (expr->data.unary->op)
        {
        case OP_UNARY_NEG:
            stack[index++] = -val;
            break;
        }

        printfn("[%02u] %s %I64d = %I64d", depth, tokcode_to_str(expr->data.unary->op), val, stack[index - 1]);

        break;

    case AST_EXPR_BNARY:
        test(expr->data.bnary->lhs, depth + 1);
        test(expr->data.bnary->rhs, depth + 1);

        rhs = stack[--index];
        lhs = stack[--index];

        switch (expr->data.bnary->op)
        {
        case OP_BNARY_ADD:
            stack[index++] = lhs + rhs;
            break;

        case OP_BNARY_SUB:
            stack[index++] = lhs - rhs;
            break;

        case OP_BNARY_MUL:
            stack[index++] = lhs * rhs;
            break;

        case OP_BNARY_DIV:
            stack[index++] = lhs / rhs;
            break;

        case OP_BNARY_MOD:
            stack[index++] = lhs % rhs;
            break;

        case OP_BNARY_AND:
            stack[index++] = lhs & rhs;
            break;

        case OP_BNARY_ORR:
            stack[index++] = lhs | rhs;
            break;

        case OP_BNARY_XOR:
            stack[index++] = lhs ^ rhs;
            break;
        }

        printfn("[%02u] %I64d %s %I64d = %I64d", depth, lhs, tokcode_to_str(expr->data.bnary->op), rhs, stack[index - 1]);

        break;

    case AST_EXPR_LISTS:
        test(expr->data.lists->expr, depth + 1);
        test(expr->data.lists->next, depth + 1);
        break;

    default:
        break;
    }
}

int main()
{
    char line[BUFSIZ];
    lexer_t *const lex = create_lexer(BUFSIZ, BUFSIZ);
    
    do
    {
        printf("RUN > ");
        fgets(line, BUFSIZ, stdin);
        dputs(lex->doub, line);

        ast_expr_t *expr = parse_ast_expr(lex);

        test(expr, 0);
        putln();
    } while (TRUE);
}