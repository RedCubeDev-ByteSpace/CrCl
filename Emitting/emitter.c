//
// Created by redcube on 15/01/23.
//
#include <gc/gc.h>
#include <string.h>
#include <stdio.h>
#include "emitter.h"
#include "../Binding/Builtins/builtins.h"
#include "../Binding/Symbols/parametersymbol.h"

char *EmitProgram(BoundProgram *prg) {
    Emitter *emt = &(Emitter) {
        0, 0, prg
    };

    char *start = "//crcl generated code\n#include <stdint.h>\n#include <stdbool.h>\n#include <stddef.h>\n";
    emt->Code = GC_MALLOC(strlen(start));
    memcpy(emt->Code, start, strlen(start));

    for (int i = 0; i < prg->FunctionCount; i++) {
        EmitFunction(emt, prg->FunctionSymbols[i], prg->FunctionBodies[i]);
    }

    printf("%s", emt->Code);
}

void EmitFunction(Emitter *emt, FunctionSymbol *sym, BoundBlockStatementNode *body) {
    CodeAppend(emt, EmitType(emt, sym->ReturnType));
    CodeAppend(emt, " ");
    CodeAppend(emt, sym->base.Name);
    CodeAppend(emt, "(");

    for (int i = 0; i < sym->Parameters.Count; i++) {
        CodeAppend(emt, EmitType(emt, ((ParameterSymbol*)sym->Parameters.Symbols[i])->base.Type));
        CodeAppend(emt, " ");
        CodeAppend(emt, sym->Parameters.Symbols[i]->Name);

        if (i < sym->Parameters.Count -1)
            CodeAppend(emt, ", ");
    }

    CodeAppend(emt, ")\n");
    EmitBlockStatement(emt, body);
}

// =====================================================================================================================
// Statements
// =====================================================================================================================
void EmitStatement(Emitter *emt, BoundNode *stmt) {
    switch (stmt->Type) {
        case BoundIfStatement: EmitIfStatement(emt, stmt); break;
        case BoundReturnStatement: EmitReturnStatement(emt, stmt); break;
        case BoundWhileStatement: EmitWhileStatement(emt, stmt); break;
        case BoundBreakStatement: EmitBreakStatement(emt, stmt); break;
        case BoundContinueStatement: EmitContinueStatement(emt, stmt); break;
        case BoundVariableDeclarationStatement: EmitLocalStatement(emt, stmt); break;
        case BoundBlockStatement: EmitBlockStatement(emt, stmt); break;
        case BoundExpressionStatement: EmitExpressionStatement(emt, stmt); break;
    }
}

void EmitIfStatement(Emitter *emt, BoundIfStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, "if (");
    CodeAppend(emt, EmitExpression(emt, stmt->Condition));
    CodeAppend(emt, ")\n");

    EmitStatement(emt, stmt->ThenBlock);

    if (stmt->ElseBlock != NULL) {
        CodeAppend(emt, "else\n");
        EmitStatement(emt, stmt->ElseBlock);
    }
}

void EmitReturnStatement(Emitter *emt, BoundReturnStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, "return ");

    if (stmt->ReturnValue != NULL)
        CodeAppend(emt, EmitExpression(emt, stmt->ReturnValue));
    CodeAppend(emt, ";\n");
}

void EmitWhileStatement(Emitter *emt, BoundWhileStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, "while (");
    CodeAppend(emt, EmitExpression(emt, stmt->Condition));
    CodeAppend(emt, ")\n");

    EmitStatement(emt, stmt->Body);
}

void EmitBreakStatement(Emitter *emt, BoundBreakStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, "break;\n");
}

void EmitContinueStatement(Emitter *emt, BoundContinueStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, "continue;\n");
}

void EmitLocalStatement(Emitter *emt, BoundLocalStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, EmitType(emt, stmt->Variable->base.Type));
    CodeAppend(emt, " ");
    CodeAppend(emt, stmt->Variable->base.base.Name);

    if (stmt->Initializer != NULL) {
        CodeAppend(emt, " = ");
        CodeAppend(emt, EmitExpression(emt, stmt->Initializer));
    }

    CodeAppend(emt, ";\n");
}

void EmitBlockStatement(Emitter *emt, BoundBlockStatementNode *stmt) {
    Indent(emt);
    CodeAppend(emt, "{\n");
    emt->Indentation++;

    for (int i = 0; i < stmt->Statements.Count; i++) {
        EmitStatement(emt, stmt->Statements.Statements[i]);
    }

    emt->Indentation--;
    Indent(emt);
    CodeAppend(emt, "}\n\n");
}

void EmitExpressionStatement(Emitter *emt, BoundExpressionStatementNode *stmt) {
    CodeAppend(emt, EmitExpression(emt, stmt->Expression));
    CodeAppend(emt, ";\n");
}

// =====================================================================================================================
// Expressions
// =====================================================================================================================
char *EmitExpression(Emitter *emt, BoundExpressionNode *expr) {
    char *out = "(";
    out = StringMerge(out, EmitExpressionInternal(emt, expr));
    out = StringMerge(out, ")");
    return out;
}

char *EmitExpressionInternal(Emitter *emt, BoundExpressionNode *expr) {

    switch (expr->base.Type) {
        case BoundLiteralExpression:
            return EmitLiteralExpression(emt, expr);
        case BoundAssignmentExpression:
            return EmitAssignmentExpression(emt, expr);
        case BoundCallExpression:
            return EmitCallExpression(emt, expr);
        case BoundNameExpression:
            return EmitNameExpression(emt, expr);
        case BoundBinaryExpression:
            return EmitBinaryExpression(emt, expr);
    }
}

char *EmitLiteralExpression(Emitter *emt, BoundLiteralExpressionNode *expr) {
    char *out;

    if (typcmp(expr->base.Type, Int8Ptr) && expr->Value != NULL) {
        int len = strlen(expr->Value);
        out = GC_MALLOC(2 + len + 1);
        out[0] = '"';
        out[1 + len] = '"';
        out[2 + len] = 0;

        memcpy(&out[1], expr->Value, len);
    }

    else if (typcmp(expr->base.Type, Int32)) {
        int len = snprintf(NULL, 0, "%d", *((int*)expr->Value));
        out = GC_MALLOC(len + 1);
        snprintf(out, len + 1, "%d", *((int*)expr->Value));
    }

    else if (typcmp(expr->base.Type, Int8Ptr) && expr->Value == NULL) {
        out = "NULL";
    }

    return out;
}

char *EmitAssignmentExpression(Emitter *emt, BoundAssignmentExpressionNode *expr) {
    char *out = StringMerge(expr->Variable->base.Name, " = ");
    StringMerge(out, EmitExpression(emt, expr->Value));

    return out;
}

char *EmitCallExpression(Emitter *emt, BoundCallExpressionNode *expr) {
    char *out = StringMerge(expr->Receiver->base.Name, "(");

    for (int i = 0; i < expr->Args.Count; i++) {
        out = StringMerge(out, EmitExpression(emt, expr->Args.NodeBuffer[i]));
        if (i < expr->Args.Count - 1)
            out = StringMerge(out, ", ");
    }

    out = StringMerge(out, ")");

    return out;
}

char *EmitNameExpression(Emitter *emt, BoundNameExpressionNode *expr) {
    return expr->Variable->base.Name;
}

char *EmitBinaryExpression(Emitter *emt, BoundBinaryExpressionNode *expr) {
    char *left = EmitExpression(emt, expr->Left);
    char *right = EmitExpression(emt, expr->Right);

    char *out = StringMerge(left, " ");

    switch (expr->Operator.Operator) {
        case PlusOperator: out = StringMerge(out, "+"); break;
        case MinusOperator: out = StringMerge(out, "-"); break;
        case MultiplyOperator: out = StringMerge(out, "*"); break;
        case DivideOperator: out = StringMerge(out, "/"); break;
        case EqualOperator: out = StringMerge(out, "=="); break;
        case UnequalOperator: out = StringMerge(out, "!="); break;
        case GreaterThanOperator: out = StringMerge(out, ">"); break;
        case GreaterEqualsOperator: out = StringMerge(out, ">="); break;
        case LessThanOperator: out = StringMerge(out, "<"); break;
        case LessEqualsOperator: out = StringMerge(out, "<="); break;
        case AndOperator: out = StringMerge(out, "&"); break;
        case BitwiseAndOperator: out = StringMerge(out, "&&"); break;
        case OrOperator: out = StringMerge(out, "|"); break;
        case BitwiseOrOperator: out = StringMerge(out, "||"); break;
    }

    out = StringMerge(out, " ");
    out = StringMerge(out, right);

    return out;
}

// =====================================================================================================================
// Other
// =====================================================================================================================

char *EmitType(Emitter *emt, TypeSymbol *typ) {
    if (typcmp(typ, Boolean))
        return "bool";
    else if (typcmp(typ, Int8))
        return "int8_t";
    else if (typcmp(typ, Int16))
        return "int16_t";
    else if (typcmp(typ, Int32))
        return "int32_t";
    else if (typcmp(typ, Int64))
        return "int64_t";
    else if (typcmp(typ, Void))
        return "void";

    if (strcmp(typ->base.Name, "ptr") == 0)
        return StringMerge(EmitType(emt, typ->Subtypes.Symbols[0]), "*");

    if (strcmp(typ->base.Name, "arr") == 0)
        return StringMerge(EmitType(emt, typ->Subtypes.Symbols[0]), "[]");

    return "";
}