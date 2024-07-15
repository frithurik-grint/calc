#include "calc.h"

int main()
{
    char buf[BUFSIZ] = { 0 };

    fgets(buf, BUFSIZ, stdin);

    dbuf_t *dbuf = create_dbuf(buf, BUFSIZ);

    int c;

    getbufc(dbuf);

    char *n = accept(dbuf);

    do
        c = putchar(getbufc(dbuf));
    while (c != EOF);

    return 0;
}