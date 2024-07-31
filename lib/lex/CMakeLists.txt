set(SOURCES
    "source.c"
    "tokens.c"
    "scanner.c"
    "lexer.c"
)

if(NOT CALC_BUILD_DYNAMIC_LIBRARY)
    add_library(lex STATIC ${SOURCES})
else()
    add_library(lex SHARED ${SOURCES})
endif()
