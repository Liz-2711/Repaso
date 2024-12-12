cmake_minimum_required (VERSION 3.10)
project (Parser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Run FA2LEXER to generate the lexer
add_custom_command (
  OUTPUT lexer.cpp
  COMMAND fa2lexer --src lexer.cpp --hdr tokens.hpp --tpl ${PROJECT_SOURCE_DIR}/lexer.fa2l
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/lexer.fa2l
  DEPENDS ${PROJECT_SOURCE_DIR}/fa/Tokens.jff
          ${PROJECT_SOURCE_DIR}/fa/Operators.jff
          ${PROJECT_SOURCE_DIR}/fa/Symbols.jff
          ${PROJECT_SOURCE_DIR}/fa/Boolean.jff
  )

add_executable(${PROJECT_NAME}  lexer2.cpp
                                parser.cpp
                                main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

