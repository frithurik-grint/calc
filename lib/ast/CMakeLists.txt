set(SOURCES
    "term.c"
    "expr.c"
    "decl.c"
    "stmt.c"
    "prgm.c"
)

if(NOT CALC_BUILD_DYNAMIC_LIBRARY)
    add_library(ast STATIC ${SOURCES})
else()
    add_library(ast SHARED ${SOURCES})
endif()
