cmake_minimum_required(VERSION 3.10.0)
project("calc-project" VERSION 1.0.4)

enable_language(C)

set(CMAKE_C_STANDARD          90)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "calc-")
set(CMAKE_STATIC_LIBRARY_PREFIX "calc-")

option(_CALC_USE_DYNAMIC_LIBRARY "Build shared libraries."             ON)
option(_CALC_USE_UNICODE_MESSAGE "Build using unicode error messages." ON)

add_compile_definitions(
    "_CALC_USE_DYNAMIC_LIBRARY=${_CALC_USE_DYNAMIC_LIBRARY}"
    "_CALC_USE_UNICODE_MESSAGE=${_CALC_USE_UNICODE_MESSAGE}"
)

add_subdirectory("")