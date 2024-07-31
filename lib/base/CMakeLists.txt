set(SOURCES
    "stddef.c"
    "stdout.c"
    "stderr.c"
    "stdmem.c"
    "stdin.c"
    "stdty.c"
    "stdbuf.c"
    "stdch.c"
    "stdstr.c"
    "stdtab.c"
)

if(NOT CALC_BUILD_DYNAMIC_LIBRARY)
    add_library(base STATIC ${SOURCES})
else()
    add_library(base SHARED ${SOURCES})
endif()
