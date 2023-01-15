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

// =====================================================================================================================
// Mehmborhs
// =====================================================================================================================
FunctionSymbol *BindFunctionDeclaration(Binder *bin, FunctionMemberNode *fnc) {
    TypeSymbol *returnType;

    if (fnc->ReturnType == NULL) // no type clause -> void
        returnType = Void;
    else
        returnType = LookupType(bin, fnc->ReturnType, true);


    ParameterSymbol *parameters[fnc->Parameters->Count];

    for (int i = 0; i < fnc->Parameters->Count; i++) {
        // wat typ?
        TypeSymbol *paramType = LookupType(bin, fnc->Parameters->Parameters[i].TypeClause, false);

        // create a parameter symbol
        parameters[i] = GC_MALLOC(sizeof(ParameterSymbol));
        parameters[i]->base.Type = ParameterSymbolType;
        parameters[i]->base.Name = fnc->Parameters->Parameters[i].Identifier.Text;

        parameters[i]->UniqueId = GetUniqueId();
        parameters[i]->Ordinal = i;
        parameters[i]->Type = paramType;
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
    BoundNode *condition = BindExpression(bin, stmt->Condition);
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
    BoundNode *returnValue = NULL;
    if (stmt->ReturnValue != NULL)
        returnValue = BindExpression(bin, stmt->ReturnValue);

    BoundReturnStatementNode *me = GC_MALLOC(sizeof(BoundReturnStatementNode));
    me->base.Type = BoundReturnStatement;
    me->ReturnValue = returnValue;

    return me;
}

BoundWhileStatementNode *BindWhileStatement(Binder *bin, WhileStatementNode *stmt) {
    BoundNode *condition = BindExpression(bin, stmt->Condition);
    BoundNode *body = BindStatement(bin, stmt->Body);

    BoundWhileStatementNode *me = GC_MALLOC(sizeof(BoundWhileStatementNode));
    me->base.Type = BoundReturnStatement;
    me->Condition = condition;
    me->Body = body;

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
    sym->base.Type = LocalVariableSymbolType;
    sym->base.Name = stmt->Identifier.Text;
    sym->UniqueId = GetUniqueId();
    sym->Type = varType;

    if (!TryRegisterSymbol(bin->ActiveScope, sym))
        Die("Failed to register local variable symbol %s, symbol already exists.", sym->base.Name);

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

    // TODO(redcube): add expression type checking
    // only call and assignment expressions are allowed to be used like statements

    BoundExpressionStatementNode *me = GC_MALLOC(sizeof(BoundExpressionStatementNode));
    me->base.Type = BoundExpressionStatement;
    me->Expression = expr;

    return me;
}

// =====================================================================================================================
// Expressions
// =====================================================================================================================
BoundExpressionNode *BindExpression(Binder *bin, Node *expr) {

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