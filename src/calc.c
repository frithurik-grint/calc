#include "calc.h"

int main()
{
    char *lexm, line[BUFSIZ];
    doub_t *src = create_doub(NULL, BUFSIZ);

    do
    {
        printf("> ");
        fgets(line, BUFSIZ, stdin);

        doub_puts(src, line);
        doub_retreat(src);

        tokcode_t c;

        do
        {
            if ((c = gettok(src, &lexm)) > TOK_ENDOS)
                doub_advance(src);

            printf(tokcode_to_str(c), lexm);
        } while (c != TOK_ENDOS);

        printf("\n");
    } while (TRUE);
}

#ifdef _CALC_BUILD_AS_ONE
#   include "calc-liber.c"

#   if CALC_PARSE_H_ == 1
#       include "calc-parse.c"
#   endif // CALC_PARSE_H_

#   if CALC_VIRTM_H_ == 1
#       include "calc-virtm.c"
#   endif // CALC_VIRTM_H_

#   include "calc-build.c"

#   if CALC_SCRPT_H_ == 1
#       include "calc-scrpt.c"
#   endif // CALC_SCRPT_H_

#   if CALC_SHELL_H_ == 1
#       include "calc-shell.c"
#   endif // CALC_SHELL_H_
#endif // _CALC_BUILD_AS_ONE