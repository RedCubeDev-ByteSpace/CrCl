//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BINDER_H
#define CRCL_BINDER_H

#include "Symbols/functionsymbol.h"
#include "Scope/scope.h"
#include "../Parsing/Nodes/Members/FunctionMember/function.h"
#include "Nodes/boundnode.h"
#include "Nodes/Statements/IfStatement/ifstatement.h"
#include "../Parsing/Nodes/Statements/IfStatement/ifstatement.h"
#include "Nodes/Statements/ReturnStatement/returnstatement.h"
#include "../Parsing/Nodes/Statements/ReturnStatement/returnstatement.h"
#include "Nodes/Statements/WhileStatement/whilestatement.h"
#include "../Parsing/Nodes/Statements/WhileStatement/whilestatement.h"
#include "Nodes/Statements/LocalStatement/localstatement.h"
#include "../Parsing/Nodes/Statements/LocalStatement/localstatement.h"
#include "Nodes/Statements/BlockStatement/blockstatement.h"
#include "Nodes/Statements/ExpressionStatement/expressionstatement.h"
#include "../Parsing/Nodes/Statements/ExpressionStatement/expressionstatement.h"
#include "Nodes/Expressions/LiteralExpression/literalexpression.h"
#include "../Parsing/Nodes/Expressions/LiteralExpression/literalexpression.h"
#include "Nodes/Expressions/CallExpression/callexpression.h"
#include "../Parsing/Nodes/Expressions/CallExpression/callexpression.h"
#include "Nodes/Expressions/NameExpression/nameexpression.h"
#include "../Parsing/Nodes/Expressions/NameExpression/nameexpression.h"
#include "Nodes/Expressions/BinaryExpression/binaryexpression.h"
#include "Nodes/Statements/BreakStatement/breakstatement.h"
#include "Nodes/Statements/ContinueStatement/continuestatement.h"
#include "Nodes/Expressions/AssignmentExpression/assignmentexpression.h"
#include "../Parsing/Nodes/Expressions/AssignmentExpression/assignmentexpression.h"
#include "../Parsing/Nodes/Members/ExternalFunctionMember/external.h"

// ============================================================================
// ENUMS
// ============================================================================
typedef enum BOUND_NODE_TYPE {
    // Statements
    BoundIfStatement,
    BoundReturnStatement,
    BoundWhileStatement,
    BoundBreakStatement,
    BoundContinueStatement,
    BoundVariableDeclarationStatement,
    BoundBlockStatement,
    BoundExpressionStatement,

    // Expressions
    BoundLiteralExpression,
    BoundAssignmentExpression,
    BoundCallExpression,
    BoundNameExpression,
    BoundUnaryExpression,
    BoundBinaryExpression,
} BoundNodeType;

// ============================================================================
// STRUCTS
// ============================================================================

typedef struct BINDER Binder;
typedef struct BOUND_PROGRAM BoundProgram;

struct BINDER {
    FunctionSymbol *CurrentFunction;
    Scope *ActiveScope;
};

struct BOUND_PROGRAM {
    FunctionSymbol **FunctionSymbols;
    BoundBlockStatementNode **FunctionBodies;
    int FunctionCount;
};

// ============================================================================
// Cool ass functions
// ============================================================================
BoundProgram *BindMembers(NodeList members);
FunctionSymbol *BindFunctionDeclaration(Binder *bin, FunctionMemberNode *fnc);
FunctionSymbol *BindExternalFunctionDeclaration(Binder *bin, ExternalFunctionMemberNode *fnc);

BoundNode *BindStatement(Binder *bin, Node *stmt);
BoundIfStatementNode *BindIfStatement(Binder *bin, IfStatementNode *stmt);
BoundReturnStatementNode *BindReturnStatement(Binder *bin, ReturnStatementNode *stmt);
BoundWhileStatementNode *BindWhileStatement(Binder *bin, WhileStatementNode *stmt);
BoundBreakStatementNode *BindBreakStatement(Binder *bin, WhileStatementNode *stmt);
BoundContinueStatementNode *BindContinueStatement(Binder *bin, WhileStatementNode *stmt);
BoundLocalStatementNode *BindLocalStatement(Binder *bin, LocalStatementNode *stmt);
BoundBlockStatementNode *BindBlockStatement(Binder *bin, BlockStatementNode *stmt);
BoundExpressionStatementNode *BindExpressionStatement(Binder *bin, ExpressionStatementNode *stmt);

BoundExpressionNode *BindExpression(Binder *bin, Node *expr);
BoundLiteralExpressionNode *BindLiteralExpression(Binder *bin, LiteralExpressionNode *expr);
BoundAssignmentExpressionNode *BindAssignmentExpression(Binder *bin, AssignmentExpressionNode *expr);
BoundCallExpressionNode *BindCallExpression(Binder *bin, CallExpressionNode *expr);
BoundNameExpressionNode *BindNameExpression(Binder *bin, NameExpressionNode *expr);
BoundBinaryExpressionNode *BindBinaryExpression(Binder *bin, BinaryExpressionNode *expr);

TypeSymbol *LookupType(Binder *bin, TypeClause *clause, bool allowLookupFailing);
TypeSymbol *LookupPrimitiveType(Binder *bin, char *name, bool allowLookupFailing);

#endif //CRCL_BINDER_H
