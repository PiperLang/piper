cmake_minimum_required(VERSION 3.13)
project(piper)

set(CMAKE_BUILD_TYPE Debug)

add_executable(piper-text-to-token
    src/piper-text-to-token.cpp
    src/stage_filetotokens.cpp
    src/scanner.cpp
)
target_include_directories(piper-text-to-token PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)

add_executable(piper-token-to-cst
    src/piper-token-to-cst.cpp
    src/scanner.cpp
)
target_include_directories(piper-token-to-cst PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)

add_executable(piper
    src/piper.cpp
    src/scanner.cpp
    src/stage_filetotokens.cpp
    src/stage_tokenstotree.cpp
    src/statements/import_statement.cpp
    src/statements/function_statement.cpp
    src/statements/block_statement.cpp
    src/statements/if_statement.cpp
    src/statements/expression_statement.cpp
    src/statements/return_statement.cpp
    
    src/expressions/add_expression.cpp
    src/expressions/call_expression.cpp
    src/expressions/double_colon_expression.cpp
    src/expressions/format_string_expression.cpp
    src/expressions/greater_than_expression.cpp
    src/expressions/identifier_expression.cpp
    src/expressions/less_than_expression.cpp
    src/expressions/number_expression.cpp    
    src/expressions/sub_expression.cpp
)
target_include_directories(piper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/includes)