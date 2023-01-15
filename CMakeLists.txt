cmake_minimum_required(VERSION 3.24)
project(CrCL C)

set(CMAKE_C_STANDARD 99)
link_libraries(gc)

add_executable(CrCL main.c Lexing/lexer.c Lexing/lexer.h Lexing/helpers.c Error/error.h Error/error.c Parsing/parser.h Parsing/parser.c Parsing/helpers.c Parsing/Nodes/Members/FunctionMember/function.h Parsing/Nodes/Members/FunctionMember/function.c Parsing/Nodes/typeclause.h Parsing/Nodes/node.h Parsing/Nodes/node.h Parsing/Nodes/Statements/BlockStatement/blockstatement.h Parsing/Nodes/Statements/IfStatement/ifstatement.h Parsing/Nodes/Statements/LocalStatement/localstatement.h Parsing/Nodes/Expressions/LiteralExpression/literalexpression.h Parsing/Nodes/Expressions/AssignmentExpression/assignmentexpression.h Parsing/Nodes/Statements/ExpressionStatement/expressionstatement.h Parsing/Nodes/Expressions/CallExpression/callexpression.h Parsing/Nodes/Expressions/NameExpression/nameexpression.h Parsing/Nodes/Expressions/BinaryExpression/binaryexpression.h Parsing/Nodes/Expressions/BinaryExpression/binaryexpression.c Parsing/Nodes/Statements/ReturnStatement/returnstatement.h Parsing/Nodes/Statements/WhileStatement/whilestatement.h Parsing/Nodes/Expressions/ParenthesizedExpression/parenthesizedexpression.h)

