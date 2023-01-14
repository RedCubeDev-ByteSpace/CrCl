cmake_minimum_required(VERSION 3.24)
project(CrCL C)

set(CMAKE_C_STANDARD 99)

add_executable(CrCL main.c Lexing/lexer.c Lexing/lexer.h Lexing/helpers.c Error/error.h Error/error.c)
