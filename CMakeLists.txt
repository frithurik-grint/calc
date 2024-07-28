cmake_minimum_required(VERSION 3.10.0)
project("calc-project" VERSION 1.0.4)

cmake_policy(SET CMP0079 NEW)

enable_language(C)

set(CMAKE_C_STANDARD          90)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "calc-")
set(CMAKE_STATIC_LIBRARY_PREFIX "calc-")
set(CMAKE_IMPORT_LIBRARY_PREFIX "calc-")

option(_CALC_USE_DYNAMIC_LIBRARY "Build shared libraries."             ON)
option(_CALC_USE_UNICODE_MESSAGE "Build using unicode error messages." ON)

add_compile_definitions(
    "_CALC_USE_DYNAMIC_LIBRARY=${_CALC_USE_DYNAMIC_LIBRARY}"
    "_CALC_USE_UNICODE_MESSAGE=${_CALC_USE_UNICODE_MESSAGE}"
)

include_directories("include")

add_compile_definitions(
    "_CALC_BUILD=1"
)

add_subdirectory("lib")
add_subdirectory("src")

target_link_libraries(calc PUBLIC liber)

install(TARGETS calc liber DESTINATION "bin")