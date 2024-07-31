set(SOURCES
    "eval.c"
    "parser.c"
)

if(NOT CALC_BUILD_DYNAMIC_LIBRARY)
    add_library(parse STATIC ${SOURCES})
else()
    add_library(parse SHARED ${SOURCES})
endif()
