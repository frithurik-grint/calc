cmake_minimum_required(VERSION 3.10.0)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	project("calc-lexlib")
endif()

set(SOURCES
	"lex.c"
)

set(WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(_CALC_USE_DYNAMIC_LIBRARY)
	add_library(lex SHARED ${SOURCES})
else()
	add_library(lex STATIC ${SOURCES})
endif()