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

// ============================================================================
// ENUMS
// ============================================================================
typedef enum BOUND_NODE_TYPE {
    // Statements
    BoundIfStatement,
    BoundReturnStatement,
    BoundWhileStatement,
    BoundVariableDeclarationStatement,
    BoundBlockStatement,
    BoundExpressionStatement,

    // Expressions
    BoundParenthesizedExpression,
    BoundLiteralExpression,
    BoundCallExpression,
    BoundNameExpression,
    BoundUnaryExpression,
    BoundBinaryExpression,
} BoundNodeType;

// ============================================================================
// STRUCTS
// ============================================================================

typedef struct BINDER Binder;

struct BINDER {
    FunctionSymbol *CurrentFunction;
    Scope *ActiveScope;
};

// ============================================================================
// Cool ass functions
// ============================================================================
FunctionSymbol *BindFunctionDeclaration(Binder *bin, FunctionMemberNode *fnc);

BoundNode *BindStatement(Binder *bin, Node *stmt);
BoundIfStatementNode *BindIfStatement(Binder *bin, IfStatementNode *stmt);
BoundReturnStatementNode *BindReturnStatement(Binder *bin, ReturnStatementNode *stmt);
BoundWhileStatementNode *BindWhileStatement(Binder *bin, WhileStatementNode *stmt);
BoundLocalStatementNode *BindLocalStatement(Binder *bin, LocalStatementNode *stmt);
BoundBlockStatementNode *BindBlockStatement(Binder *bin, BlockStatementNode *stmt);
BoundExpressionStatementNode *BindExpressionStatement(Binder *bin, ExpressionStatementNode *stmt);

BoundExpressionNode *BindExpression(Binder *bin, Node *expr);

TypeSymbol *LookupType(Binder *bin, TypeClause *clause, bool allowLookupFailing);
TypeSymbol *LookupPrimitiveType(Binder *bin, char *name, bool allowLookupFailing);

#endif //CRCL_BINDER_H
