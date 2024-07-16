#include "calc.h"

int main()
{

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