cmake_minimum_required(VERSION 3.20.0)
project(calc VERSION 1.0.5 LANGUAGES C)

set(CMAKE_C_STANDARD          90)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_IMPORT_LIBRARY_PREFIX "calc-")
set(CMAKE_SHARED_LIBRARY_PREFIX "calc-")
set(CMAKE_STATIC_LIBRARY_PREFIX "calc-")

option(CALC_BUILD_DYNAMIC_LIBRARY "Build shared library." OFF)

if(CALC_BUILD_DYNAMIC_LIBRARY AND WIN32)
    set(WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_compile_definitions(
    "_CALC_BUILD_INLINES=1"
)

add_executable(calc "src/calc.c")

include_directories("include")

add_subdirectory("lib")
# add_subdirectory("src")
add_subdirectory("tests")
