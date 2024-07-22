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

int get_maxstack(ast_expr_t *const expr)
{
    static int index = 0, max = 0;

    switch (expr->kind)
    {
    case AST_EXPR_UNARY:
        get_maxstack(expr->data.unary->val);
        break;

    case AST_EXPR_BNARY:
        get_maxstack(expr->data.bnary->lhs);
        get_maxstack(expr->data.bnary->rhs);

        --index;
        --index;
        ++index;

        break;

    case AST_EXPR_UNSIG:
        ++index;
        break;
    }

    if (index > max)
        max = index;

    return max;
}

void intg_to_cil(FILE *const stream, long long uint)
{
    if (uint < BYTE_MAX)
        fprintfn(stream, "ldc.i4.s %d", (int)uint);
    else if (uint <= INT_MAX)
        fprintfn(stream, "ldc.i4 %d", (int)uint);
    else
        fprintfn(stream, "ldc.i8 %d", (int)uint);
}

void expr_to_cil(FILE *const stream, ast_expr_t *const expr);

void unar_to_cil(FILE *const stream, ast_expr_unary_t *const expr)
{
    expr_to_cil(stream, expr->val);

    switch (expr->op)
    {
    case OP_UNARY_NEG:
        fprintln(stream, "neg");
        break;

    default:
        break;
    }
}

void bnar_to_cil(FILE *const stream, ast_expr_bnary_t *const expr)
{
    expr_to_cil(stream, expr->lhs);
    expr_to_cil(stream, expr->rhs);

    switch (expr->op)
    {
    case OP_BNARY_ADD:
        fprintln(stream, "add");
        break;

    case OP_BNARY_SUB:
        fprintln(stream, "sub");
        break;

    case OP_BNARY_MUL:
        fprintln(stream, "mul");
        break;

    case OP_BNARY_DIV:
        fprintln(stream, "div");
        break;

    case OP_BNARY_MOD:
        fprintln(stream, "rem");
        break;

    case OP_BNARY_AND:
        fprintln(stream, "and");
        break;

    case OP_BNARY_ORR:
        fprintln(stream, "or");
        break;

    case OP_BNARY_XOR:
        fprintln(stream, "xor");
        break;

    default:
        break;
    }
}

void expr_to_cil(FILE *const stream, ast_expr_t *const expr)
{
    switch (expr->kind)
    {
    case AST_EXPR_UNSIG:
    case AST_EXPR_SIGND:
        intg_to_cil(stream, expr->data.signd);
        break;

    case AST_EXPR_UNARY:
        unar_to_cil(stream, expr->data.unary);
        break;

    case AST_EXPR_BNARY:
        bnar_to_cil(stream, expr->data.bnary);
        break;
    }
}

void to_cil(FILE *const stream, ast_expr_t *const expr)
{
    fprintln(stream, ".assembly Test {}");
    fprintln(stream, ".assembly extern mscorlib {}");
    fputln  (stream);
    fprintln(stream, ".module test.exe");
    fputln  (stream);
    fprintln(stream, ".method private hidebysig static void Main() cil managed");
    fprintln(stream, "{");
    fprintln(stream, ".entrypoint");
    fprintfn(stream, ".maxstack %d", get_maxstack(expr));
    fputln  (stream);

    expr_to_cil(stream, expr);

    fprintln(stream, "call void [mscorlib]System.Console::WriteLine(int32)");
    fprintln(stream, "ret");
    fprintln(stream, "}");
}

int main()
{
    char line[BUFSIZ];
    lexer_t *const lex = create_lexer(BUFSIZ, BUFSIZ);

    printf("TOK > ");
    fgets(line, BUFSIZ, stdin);
    dputs(lex->doub, line);

    ast_expr_t *root = parse_ast_expr_bnary(lex);

    to_cil(fopen("test.il", "w+"), root);
}