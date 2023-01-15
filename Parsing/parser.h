//
// Created by redcube on 14/01/23.
//

#ifndef CRCL_PARSER_H
#define CRCL_PARSER_H

#include "../Lexing/lexer.h"
#include "Nodes/node.h"
#include "Nodes/Members/FunctionMember/function.h"
#include "Nodes/Statements/IfStatement/ifstatement.h"
#include "Nodes/Statements/LocalStatement/localstatement.h"
#include "Nodes/Statements/ReturnStatement/returnstatement.h"
#include "Nodes/Statements/WhileStatement/whilestatement.h"
#include "Nodes/Statements/ExpressionStatement/expressionstatement.h"
#include "Nodes/Expressions/LiteralExpression/literalexpression.h"
#include "Nodes/Expressions/AssignmentExpression/assignmentexpression.h"
#include "Nodes/Expressions/NameExpression/nameexpression.h"
#include "Nodes/Expressions/CallExpression/callexpression.h"
#include "Nodes/Expressions/BinaryExpression/binaryexpression.h"
#include "Nodes/Expressions/ParenthesizedExpression/parenthesizedexpression.h"
#include "Nodes/Statements/BreakStatement/breakstatement.h"
#include "Nodes/Statements/ContinueStatement/continuestatement.h"

// ============================================================================
// ENUMS
// ============================================================================
typedef enum NODE_TYPE {
    // Members
    GlobalMember,
    FunctionMember,
    ExternalFunctionMember,
    StructMember,
    EnumMember,

    // Statements
    IfStatement,
    ReturnStatement,
    WhileStatement,
    BreakStatement,
    ContinueStatement,
    VariableDeclarationStatement,
    BlockStatement,
    ExpressionStatement,

    // Expressions
    ParenthesizedExpression,
    LiteralExpression,
    AssignmentExpression,
    CallExpression,
    NameExpression,
    UnaryExpression,
    BinaryExpression,
} NodeType;

// ============================================================================
// STRUCTS
// ============================================================================
typedef struct PARSER Parser;

// Parsing object
struct PARSER {
    TokenList Tokens;
    int CurrentPointer;

    Node **NodeBuffer;
    int BufferSize;
    int NodeCount;
};

// ============================================================================
// FUNCTIONS
// ============================================================================
Token CurrentToken(Parser *prs);
Token PeekToken(Parser *prs, int offset);
Token Consume(Parser *prs, TokenType token);
void AppendMember(Parser *prs, Node *node);

NodeList Parse(TokenList tokens);
void ParseMembers(Parser *prs);
void ParseFunctionDeclaration(Parser *prs);
void ParseExternalFunctionDeclaration(Parser *prs);
FunctionParameterList *ParseParameters(Parser *prs);
FunctionParameter *ParseParameter(Parser *prs);
TypeClause *ParseTypeClause(Parser *prs);

Node *ParseStatement(Parser *prs);
BlockStatementNode *ParseBlockStatement(Parser *prs);
IfStatementNode *ParseIfStatement(Parser *prs);
LocalStatementNode *ParseLocalStatement(Parser *prs);
ReturnStatementNode *ParseReturnStatement(Parser *prs);
WhileStatementNode *ParseWhileStatement(Parser *prs);
BreakStatementNode *ParseBreakStatement(Parser *prs);
ContinueStatementNode *ParseContinueStatement(Parser *prs);
ExpressionStatementNode *ParseExpressionStatement(Parser *prs);

Node *ParseExpression(Parser *prs);
Node *ParsePrimaryExpression(Parser *prs);

ParenthesizedExpressionNode *ParseParenthesizedExpression(Parser *prs);
LiteralExpressionNode *ParseLiteralExpression(Parser *prs);
AssignmentExpressionNode *ParseAssignmentExpression(Parser *prs);
CallExpressionNode *ParseCallExpression(Parser *prs);
NameExpressionNode *ParseNameExpression(Parser *prs);
Node *ParseBinaryExpression(Parser *prs, int precedence);

BinaryOperator ParseBinaryOperator(Parser *prs);

#endif //CRCL_PARSER_H
