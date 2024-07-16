#include "calc.h"

int main()
{
    doub_t *d = create_doub(NULL, 0);

    doub_puts(d, "a = 3 + 2");
    doub_retreat(d);

    char c = (char)doub_getc(d);

    char *id = doub_chop(d);

    doub_advance(d);

    c = (char)doub_getc(d);

    return 0;
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