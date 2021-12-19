cmake_minimum_required(VERSION 3.19)
project(scheme)

set(CMAKE_CXX_STANDARD 17)

add_library(scheme_lib main.cpp
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
target_include_directories(scheme_lib PRIVATE include/impl)
target_include_directories(scheme_lib PUBLIC include)

add_executable(scheme main.cpp)
target_link_libraries(scheme PRIVATE scheme_lib)

