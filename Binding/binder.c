//
// Created by redcube on 15/01/23.
//
#include <string.h>
#include <gc/gc.h>
#include "binder.h"
#include "../Parsing/Nodes/Members/FunctionMember/function.h"
#include "Builtins/builtins.h"
#include "../Error/error.h"
#include "Symbols/parametersymbol.h"
#include "Nodes/boundnode.h"
#include "../Parsing/parser.h"
#include "Nodes/Statements/IfStatement/ifstatement.h"
#include "../Parsing/Nodes/Statements/IfStatement/ifstatement.h"
#include "Nodes/Statements/ReturnStatement/returnstatement.h"
#include "Nodes/Statements/WhileStatement/whilestatement.h"
#include "Nodes/Statements/LocalStatement/localstatement.h"
#include "Nodes/Statements/BlockStatement/blockstatement.h"
#include "Nodes/Statements/ExpressionStatement/expressionstatement.h"
#include "Nodes/Expressions/LiteralExpression/literalexpression.h"
#include "Nodes/Expressions/CallExpression/callexpression.h"
#include "Nodes/Expressions/NameExpression/nameexpression.h"
#include "Nodes/Expressions/BinaryExpression/binaryexpression.h"
#include "Nodes/Statements/BreakStatement/breakstatement.h"
#include "Nodes/Statements/ContinueStatement/continuestatement.h"
#include "Nodes/Expressions/AssignmentExpression/assignmentexpression.h"

// =====================================================================================================================
// Mehmborhs
// =====================================================================================================================
BoundProgram *BindMembers(NodeList members) {
    // create a root (global) scope
    Scope *rootScope = &(Scope) {
            0, 0, 0, 0
    };

    // create a new root binder
    Binder *bin = &(Binder) {
            0, rootScope,
    };

    FunctionSymbol  *functionSymbols[members.Count];
    BoundBlockStatementNode *functionBodies[members.Count];

    // declare all functions
    for (int i = 0; i < members.Count; i++) {
        functionSymbols[i] = BindFunctionDeclaration(bin, members.NodeBuffer[i]);
        TryRegisterSymbol(rootScope, functionSymbols[i]);
    }

    // bind all function bodies
    for (int i = 0; i < members.Count; i++) {
        // create a scope for the function
        Scope *funcScope = &(Scope) {
                0, 0, 0, rootScope
        };

        // create a binder for the function
        Binder *fncBin = &(Binder) {
                functionSymbols[i], funcScope,
        };

        // bind the body
        functionBodies[i] = BindStatement(fncBin, ((FunctionMemberNode*)members.NodeBuffer[i])->Body);
    }

    // put all of our results into a bound program object
    BoundProgram *boundProgram = GC_MALLOC(sizeof(BoundProgram));
    boundProgram->FunctionSymbols = GC_malloc(sizeof(FunctionSymbol*) * members.Count);
    boundProgram->FunctionBodies = GC_malloc(sizeof(BoundBlockStatementNode*) * members.Count);
    boundProgram->FunctionCount = members.Count;

    memcpy(boundProgram->FunctionSymbols, functionSymbols, sizeof(FunctionSymbol*) * members.Count);
    memcpy(boundProgram->FunctionBodies, functionBodies, sizeof(BoundBlockStatementNode*) * members.Count);

    return boundProgram;
}

FunctionSymbol *BindFunctionDeclaration(Binder *bin, FunctionMemberNode *fnc) {
    TypeSymbol *returnType;

    if (fnc->ReturnType == NULL) // no type clause -> void
        returnType = Void;
    else
        returnType = LookupType(bin, fnc->ReturnType, true);


    ParameterSymbol *parameters[fnc->Parameters->Count];

    for (int i = 0; i < fnc->Parameters->Count; i++) {
        // wat typ?
        TypeSymbol *paramType = LookupType(bin, fnc->Parameters->Parameters[i]->TypeClause, false);

        // create a parameter symbol
        parameters[i] = GC_MALLOC(sizeof(ParameterSymbol));
        parameters[i]->base.Type = ParameterSymbolType;
        parameters[i]->base.base.Name = fnc->Parameters->Parameters[i]->Identifier.Text;

        parameters[i]->UniqueId = GetUniqueId();
        parameters[i]->Ordinal = i;
        parameters[i]->base.Type = paramType;
    }

    SymbolList parameterList;
    parameterList.Count = fnc->Parameters->Count;
    parameterList.Symbols = GC_MALLOC(sizeof(ParameterSymbol*) * fnc->Parameters->Count);
    memcpy(parameterList.Symbols, parameters, sizeof(ParameterSymbol*) * fnc->Parameters->Count);

    FunctionSymbol *me = GC_MALLOC(sizeof(FunctionSymbol));
    me->base.Type = FunctionSymbolType;
    me->base.Name = fnc->Identifier.Text;
    me->ReturnType = returnType;
    me->Parameters = parameterList;

    return me;
}

// =====================================================================================================================
// Statements
// =====================================================================================================================
BoundNode *BindStatement(Binder *bin, Node *stmt) {
    switch (stmt->Type) {
        case IfStatement:
            return BindIfStatement(bin, stmt);
        case ReturnStatement:
            return BindReturnStatement(bin, stmt);
        case WhileStatement:
            return BindWhileStatement(bin, stmt);
        case BreakStatement:
            return BindBreakStatement(bin, stmt);
        case ContinueStatement:
            return BindContinueStatement(bin, stmt);
        case VariableDeclarationStatement:
            return BindLocalStatement(bin, stmt);
        case BlockStatement:
            return BindBlockStatement(bin, stmt);
        case ExpressionStatement:
            return BindExpressionStatement(bin, stmt);

        default:
            Die("Unknown node type :( (%d)", stmt->Type);
    }
}

BoundIfStatementNode *BindIfStatement(Binder *bin, IfStatementNode *stmt) {
    BoundExpressionNode *condition = BindExpression(bin, stmt->Condition);

    if (!typcmp(condition->Type, Boolean)) {
        Die("Expression inside if statement needs to be of type bool!");
    }

    BoundNode *thenBlock = BindStatement(bin, stmt->ThenBlock);
    BoundNode *elseBlock = NULL;

    if (stmt->ElseBlock != NULL)
        elseBlock = BindStatement(bin, stmt->ElseBlock);

    BoundIfStatementNode *me = GC_MALLOC(sizeof(BoundIfStatementNode));
    me->base.Type = BoundIfStatement;
    me->Condition = condition;
    me->ThenBlock = thenBlock;
    me->ElseBlock = elseBlock;

    return me;
}

BoundReturnStatementNode *BindReturnStatement(Binder *bin, ReturnStatementNode *stmt) {
    if (typcmp(bin->CurrentFunction->ReturnType, Void)) {
        if (stmt->ReturnValue != NULL)
            Die("Return value given in a void function!");
    } else {
        if (stmt->ReturnValue == NULL)
            Die("No return value given in a non-void function!");
    }

    BoundExpressionNode *returnValue = NULL;
    if (stmt->ReturnValue != NULL)
        returnValue = BindExpression(bin, stmt->ReturnValue);

    if (!typcmp(returnValue->Type, bin->CurrentFunction->ReturnType))
        Die("Function return value type doesnt match its return type!");

    BoundReturnStatementNode *me = GC_MALLOC(sizeof(BoundReturnStatementNode));
    me->base.Type = BoundReturnStatement;
    me->ReturnValue = returnValue;

    return me;
}

BoundWhileStatementNode *BindWhileStatement(Binder *bin, WhileStatementNode *stmt) {
    BoundExpressionNode *condition = BindExpression(bin, stmt->Condition);

    if (!typcmp(condition->Type, Boolean)) {
        Die("Expression inside while statement needs to be of type bool!");
    }

    BoundNode *body = BindStatement(bin, stmt->Body);

    BoundWhileStatementNode *me = GC_MALLOC(sizeof(BoundWhileStatementNode));
    me->base.Type = BoundWhileStatement;
    me->Condition = condition;
    me->Body = body;

    return me;
}

BoundBreakStatementNode *BindBreakStatement(Binder *bin, WhileStatementNode *stmt) {
    BoundBreakStatementNode *me = GC_MALLOC(sizeof(BoundBreakStatementNode));
    me->base.Type = BoundBreakStatement;
    return me;
}

BoundContinueStatementNode *BindContinueStatement(Binder *bin, WhileStatementNode *stmt) {
    BoundContinueStatementNode *me = GC_MALLOC(sizeof(BoundContinueStatementNode));
    me->base.Type = BoundContinueStatement;
    return me;
}

BoundLocalStatementNode *BindLocalStatement(Binder *bin, LocalStatementNode *stmt) {

    // if we have an explicit type -> resolve it
    TypeSymbol *varType = NULL;
    if (stmt->Type != NULL)
        varType = LookupType(bin, stmt->Type, false);

    // if we have an initializer -> bind it
    BoundExpressionNode *initializer = NULL;
    if (stmt->Initializer != NULL)
        initializer = BindExpression(bin, stmt->Initializer);

    // if we dont have a type but have a value -> use the value's type
    if (varType == NULL && initializer != NULL)
        varType = initializer->Type;

    LocalVariableSymbol *sym = GC_MALLOC(sizeof(LocalVariableSymbol));
    sym->base.base.Type = LocalVariableSymbolType;
    sym->base.base.Name = stmt->Identifier.Text;
    sym->UniqueId = GetUniqueId();
    sym->base.Type = varType;

    if (!TryRegisterSymbol(bin->ActiveScope, sym))
        Die("Failed to register local variable symbol %s, symbol already exists.", sym->base.base.Name);

    BoundLocalStatementNode *me = GC_MALLOC(sizeof(BoundLocalStatementNode));
    me->base.Type = BoundVariableDeclarationStatement;
    me->Variable = sym;
    me->Initializer = initializer;

    return me;
}

BoundBlockStatementNode *BindBlockStatement(Binder *bin, BlockStatementNode *stmt) {
    Scope *prevScope = bin->ActiveScope;

    Scope *newScope = GC_MALLOC(sizeof(Scope));
    newScope->Parent = bin->ActiveScope;
    newScope->SymbolBuffer = 0;
    newScope->SymbolCount = 0;
    newScope->BufferSize = 0;

    bin->ActiveScope = newScope;

    BoundNode *statements[stmt->Statements.Count];
    for (int i = 0; i < stmt->Statements.Count; i++) {
        statements[i] = BindStatement(bin, stmt->Statements.Statements[i]);
    }

    BoundStatementList list;
    list.Count = stmt->Statements.Count;
    list.Statements = GC_MALLOC(sizeof(BoundNode*) * list.Count);
    memcpy(list.Statements, statements, sizeof(BoundNode*) * list.Count);

    bin->ActiveScope = prevScope;

    BoundBlockStatementNode *me = GC_MALLOC(sizeof(BoundBlockStatementNode));
    me->base.Type = BoundBlockStatement;
    me->Statements = list;

    return me;
}

BoundExpressionStatementNode *BindExpressionStatement(Binder *bin, ExpressionStatementNode *stmt) {
    BoundExpressionNode *expr = BindExpression(bin, stmt->Expression);

    if (expr->base.Type != BoundCallExpression && expr->base.Type != BoundAssignmentExpression)
        Die("Only call and assignment expressions are allowed to be used as statements!");

    BoundExpressionStatementNode *me = GC_MALLOC(sizeof(BoundExpressionStatementNode));
    me->base.Type = BoundExpressionStatement;
    me->Expression = expr;

    return me;
}

// =====================================================================================================================
// Expressions
// =====================================================================================================================
BoundExpressionNode *BindExpression(Binder *bin, Node *expr) {
    switch (expr->Type) {
        case ParenthesizedExpression:
            return BindExpression(bin, ((ParenthesizedExpressionNode*)expr)->Expression);

        case LiteralExpression:
            return BindLiteralExpression(bin, expr);

        case AssignmentExpression:
            return BindAssignmentExpression(bin, expr);

        case CallExpression:
            return BindCallExpression(bin, expr);

        case NameExpression:
            return BindNameExpression(bin, expr);

        case BinaryExpression:
            return BindBinaryExpression(bin, expr);

        default:
            Die("Unknown expression >:( (%d)", expr->Type);
    }
}

BoundLiteralExpressionNode *BindLiteralExpression(Binder *bin, LiteralExpressionNode *expr) {

    BoundLiteralExpressionNode *me = GC_MALLOC(sizeof(BoundLiteralExpressionNode));
    me->base.base.Type = BoundLiteralExpression;
    me->Value = expr->Literal.Value;

    switch (expr->Literal.Type) {
        case String:
            me->base.Type = Int8Ptr; break;
        case Number:
            me->base.Type = Int32; break;
        case NullKeyword:
            me->base.Type = Int8Ptr; break;
    }

    return me;
}

BoundAssignmentExpressionNode *BindAssignmentExpression(Binder *bin, AssignmentExpressionNode *expr) {
    VariableSymbol *var = TryLookupSymbol(bin->ActiveScope, expr->Identifier.Text);
    if (var == NULL)
        Die("Could not find variable '%s' in current scope!", expr->Identifier.Text);

    BoundExpressionNode *value = BindExpression(bin, expr->Value);
    if (!typcmp(var->Type, value->Type))
        Die("Variable and value type dont match!");

    BoundAssignmentExpressionNode *me = GC_MALLOC(sizeof(BoundAssignmentExpressionNode));
    me->base.base.Type = BoundAssignmentExpression;
    me->base.Type = value->Type;
    me->Value = value;
    me->Variable = var;

    return me;
}

BoundCallExpressionNode *BindCallExpression(Binder *bin, CallExpressionNode *expr) {
    // lookup function
    FunctionSymbol *func = TryLookupSymbol(bin->ActiveScope, expr->Identifier.Text);
    if (func == NULL)
        Die("Couldnt find your stupid ass function (%s)", expr->Identifier.Text);

    if (func->Parameters.Count != expr->Arguments.Count)
        Die("Function %s expects %d arguments but got %d!", func->base.Name, func->Parameters.Count, expr->Arguments.Count);

    BoundExpressionNode *args[expr->Arguments.Count];
    for (int i = 0; i < expr->Arguments.Count; i++) {
        args[i] = BindExpression(bin, expr->Arguments.NodeBuffer[i]);

        if (!typcmp(((ParameterSymbol*)func->Parameters.Symbols[i])->base.Type, args[i]->Type))
            Die("Arguments types dont match for function %s!", func->base.Name);
    }

    BoundNodeList list;
    list.Count = expr->Arguments.Count;
    list.NodeBuffer = GC_MALLOC(expr->Arguments.Count * sizeof(BoundExpressionNode*));
    memcpy(list.NodeBuffer, args, expr->Arguments.Count * sizeof(BoundExpressionNode*));

    BoundCallExpressionNode *me = GC_MALLOC(sizeof(BoundCallExpressionNode));
    me->base.base.Type = BoundCallExpression;
    me->base.Type = func->ReturnType;
    me->Receiver = func;
    me->Args = list;

    return me;
}

BoundNameExpressionNode *BindNameExpression(Binder *bin, NameExpressionNode *expr) {
    // lookup variable
    VariableSymbol *var = TryLookupSymbol(bin->ActiveScope, expr->Identifier.Text);
    if (var == NULL)
        Die("Could not locate variable called %s", expr->Identifier.Text);

    BoundNameExpressionNode *me = GC_MALLOC(sizeof(BoundNameExpressionNode));
    me->base.base.Type = BoundNameExpression;
    me->base.Type = var->Type;
    me->Variable = var;

    return me;
}

BoundBinaryExpressionNode *BindBinaryExpression(Binder *bin, BinaryExpressionNode *expr) {
    BoundExpressionNode *left = BindExpression(bin, expr->Left);
    BoundExpressionNode *right = BindExpression(bin, expr->Right);

    BoundBinaryOperator *op = BindBinaryOperator(expr->Operator, left->Type, right->Type);
    if (op == NULL)
        Die("Could not find operator of type %d for types %s and %s!", expr->Operator, left->Type->base.Name, right->Type->base.Name);

    BoundBinaryExpressionNode *me = GC_MALLOC(sizeof(BoundBinaryExpressionNode));
    me->base.base.Type = BoundBinaryExpression;
    me->base.Type = op->Result;
    me->Operator = *op;
    me->Left = left;
    me->Right = right;

    return me;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================
TypeSymbol *LookupType(Binder *bin, TypeClause *clause, bool allowLookupFailing) {

    // we werent even given a clause
    if (clause == NULL) {
        if (!allowLookupFailing)
            Die("type clause was null, probably an internal error tbh");

        return NULL;
    }

    if (strcmp(clause->Identifier.Text, "arr") == 0) {

        // is this even a valid array type??
        if (clause->SubTypes->Count != 1) {
            Die("Array types need EXACTLY one sub type. (got: %s)", clause->SubTypes->Count);
        }

        // generate a new type symbol on the fly
        TypeSymbol *array = GC_MALLOC(sizeof(TypeSymbol));
        array->base.Type = TypeSymbolType;
        array->base.Name = "arr";

        // generate sub type list
        TypeSymbol *subType = LookupType(bin, &clause->SubTypes->Clauses[0], false);
        TypeSymbolList subTypes;
        subTypes.Symbols = GC_MALLOC(sizeof(TypeSymbol*));
        subTypes.Symbols[0] = subType;
        subTypes.Count = 1;

        // store the list in our symbol
        array->Subtypes = subTypes;

        // cool
        return array;
    }

    if (strcmp(clause->Identifier.Text, "ptr") == 0) {

        // is this even a valid array type??
        if (clause->SubTypes->Count != 1) {
            Die("Pointer types need EXACTLY one sub type. (got: %s)", clause->SubTypes->Count);
        }

        // generate a new type symbol on the fly
        TypeSymbol *array = GC_MALLOC(sizeof(TypeSymbol));
        array->base.Type = TypeSymbolType;
        array->base.Name = "ptr";

        // generate sub type list
        TypeSymbol *subType = LookupType(bin, clause->SubTypes->Clauses[0], false);
        TypeSymbolList subTypes;
        subTypes.Symbols = GC_MALLOC(sizeof(TypeSymbol*));
        subTypes.Symbols[0] = subType;
        subTypes.Count = 1;

        // store the list in our symbol
        array->Subtypes = subTypes;

        // cool
        return array;
    }

    // otherwise this is probably a primitive
    return LookupPrimitiveType(bin, clause->Identifier.Text, allowLookupFailing);
}

TypeSymbol *LookupPrimitiveType(Binder *bin, char *name, bool allowLookupFailing) {
    if (strcmp(name, "bool") == 0)
        return Boolean;

    if (strcmp(name, "int8") == 0)
        return Int8;

    if (strcmp(name, "int16") == 0)
        return Int16;

    if (strcmp(name, "int32") == 0)
        return Int32;

    if (strcmp(name, "int64") == 0)
        return Int64;

    if (strcmp(name, "void") == 0)
        return Void;

    // no fucking clue what this type is supposed to be
    if (!allowLookupFailing)
        Die("Dude what the hell is type %s supposed to be??", name);

    return NULL;
}