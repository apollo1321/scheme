cmake_minimum_required(VERSION 3.19)
project(scheme)

set(CMAKE_CXX_STANDARD 17)

add_library(libscheme
        src/boolean.cpp
        src/control_flow.cpp
        src/expression.cpp
        src/common.cpp
        src/integer.cpp
        src/lambda.cpp
        src/list.cpp
        src/parser.cpp
        src/print.cpp
        src/procedure.cpp
        src/quote.cpp
        src/scheme.cpp
        src/symbol.cpp
        src/tokenizer.cpp)

target_include_directories(libscheme PRIVATE include/impl)
target_include_directories(libscheme PUBLIC include)

add_executable(scheme-repl main.cpp)
target_link_libraries(scheme-repl PRIVATE libscheme)

find_package(Catch2 3 QUIET)

if (Catch2_FOUND)
    add_subdirectory(test)
endif ()
