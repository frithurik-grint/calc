#include "calc/base/stderr.h"

/* =---- Exceptions Management ---------------------------------= */

#pragma region Exceptions Management

// +---- Internal (Exceptions Builder) -------------------------+

#pragma region Internal (Exceptions Builder)

static ex_t *_build_from_code(excode_t code)
{
    ex_t *ex = alloc(ex_t);

    switch (code)
    {
    default:
    case EX_UNHDL:
        ex->name = "UnhandledException";                // exception display name
        ex->mssg = "An exception has been thrown.";     // default exception message
        ex->kind = EX_KIND_INTRNL;                      // exception kind
        break;

    case EX_UNRCH:
        ex->name = "UnreachableException";
        ex->mssg = "Unreachable code has been reached.";
        ex->kind = EX_KIND_INTRNL;
        break;
    }

    return ex;
}

#pragma endregion

// +---- Internal (Exceptions Builder) -- End ------------------+

ex_t *_CDECL create_exception(char *const message, excode_t code)
{
    ex_t *ex = _build_from_code(code);

    if (message)
        ex->mssg = message;

    ex->file = NULL;
    ex->func = NULL;
    ex->line = 0;
    ex->prev = NULL;

    return ex;
}

ex_t *_CDECL create_exception_located(char *const message, excode_t code, char *const func, char *const file, unsigned int line)
{
    ex_t *ex = _build_from_code(code);

    if (message)
        ex->mssg = message;

    ex->file = file;
    ex->func = func;
    ex->line = line;
    ex->prev = NULL;

    return ex;
}

ex_t *_CDECL delete_exception(ex_t *const exception)
{
    ex_t *prev = exception->prev;

    free(exception);

    return prev;
}

// Exception Stack

ex_stack_t *_CDECL create_ex_stack()
{
    ex_stack_t *stack = alloc(ex_stack_t);

    stack->extcd = 0;
    stack->count = 0;
    stack->excpt = NULL;

    return stack;
}

void _CDECL delete_ex_stack(ex_stack_t *const stack)
{
    ex_t *ex = stack->excpt;

    while (ex)
        ex = delete_exception(ex);

    free(stack);

    return;
}

#ifndef _CALC_MINIMAL_BUILD

void _CDECL ex_stack_dump(FILE *const stream, ex_stack_t *const stack)
{
    if (stack->count == 0)
        return;

    ex_t *ex = stack->excpt;

    switch (stack->extcd)
    {
    case EXIT_SUCCESS:
        fprintfn(stream, "Process exited with %u exceptions:", stack->count);
        break;

    case EXIT_FAILURE:
        fprintfn(stream, "Process faulted with %u exceptions:", stack->count);
        break;

    case EXIT_ABORTED:
        fprintfn(stream, "Process aborted with %u exceptions:", stack->count);
        break;

    default:
        fprintfn(stream, "Process terminated (exit code: %u) with %u exceptions:", stack->extcd, stack->count);
        break;
    }

    while (ex)
        fprintfn(stream, "    %s: %s at %s in %s:%u", ex->name, ex->mssg, ex->func, ex->file, ex->line), ex = ex->prev;

    return;
}

#endif // _CALC_MINIMAL_BUILD

void _CDECL pushex(ex_stack_t *const stack, ex_t *const exception)
{
    exception->prev = stack->excpt;

    stack->excpt = exception;
    ++stack->count;

    return;
}

ex_t *_CDECL popex(ex_stack_t *const stack)
{
    ex_t *ex = stack->excpt;

    if (ex)
        stack->excpt = ex->prev, --stack->count;

    return ex;
}

ex_t *_CDECL peekex(ex_stack_t *const stack)
{
    return stack->excpt;
}

void _CDECL _ex_throw(ex_stack_t *const stack, const char *const file, const char *const func, unsigned int line, excode_t code, const char *const message)
{
    if (stack->extcd == EXIT_SUCCESS)
        stack->excpt = EXIT_FAILURE;

    pushex(stack, create_exception_located(message, code, func, file, line));

    return;
}

void _CDECL _ex_throwf(ex_stack_t *const stack, const char *const file, const char *const func, unsigned int line, excode_t code, const char *const format, ...)
{
    if (stack->extcd == EXIT_SUCCESS)
        stack->excpt = EXIT_FAILURE;

    va_list arglist;

    va_start(arglist, format);

    pushex(stack, create_exception_located(vformatn(format, arglist), code, func, file, line));

    va_end(arglist);

    return;
}

int _CDECL except(ex_stack_t *const stack, excode_t code, ex_callback_t callback)
{
    ex_t *ex;

    if (stack->excpt->code == code)
        return callback(popex(stack));
    else
        return -1;
}

#pragma endregion

/* =------------------------------------------------------------= */
