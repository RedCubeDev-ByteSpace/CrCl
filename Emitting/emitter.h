//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_EMITTER_H
#define CRCL_EMITTER_H

#include "../Binding/binder.h"

#define TAB_SIZE 4

typedef struct EMITTER {
    char *Code;
    int Indentation;
    BoundProgram *Program;
} Emitter;

char *StringMerge(char *a, char *b);
void CodeAppend(Emitter *emt, char *a);
void Indent(Emitter *emt);

char *EmitProgram(BoundProgram *prg);
void EmitFunction(Emitter *emt, FunctionSymbol *sym, BoundBlockStatementNode *body);

void EmitIfStatement(Emitter *emt, BoundIfStatementNode *stmt);
void EmitReturnStatement(Emitter *emt, BoundReturnStatementNode *stmt);
void EmitWhileStatement(Emitter *emt, BoundWhileStatementNode *stmt);
void EmitBreakStatement(Emitter *emt, BoundBreakStatementNode *stmt);
void EmitContinueStatement(Emitter *emt, BoundContinueStatementNode *stmt);
void EmitLocalStatement(Emitter *emt, BoundLocalStatementNode *stmt);
void EmitBlockStatement(Emitter *emt, BoundBlockStatementNode *stmt);
void EmitExpressionStatement(Emitter *emt, BoundExpressionStatementNode *stmt);

char *EmitExpression(Emitter *emt, BoundExpressionNode *expr);
char *EmitExpressionInternal(Emitter *emt, BoundExpressionNode *expr);
char *EmitLiteralExpression(Emitter *emt, BoundLiteralExpressionNode *expr);
char *EmitAssignmentExpression(Emitter *emt, BoundAssignmentExpressionNode *expr);
char *EmitCallExpression(Emitter *emt, BoundCallExpressionNode *expr);
char *EmitNameExpression(Emitter *emt, BoundNameExpressionNode *expr);
char *EmitBinaryExpression(Emitter *emt, BoundBinaryExpressionNode *expr);

char *EmitType(Emitter *emt, TypeSymbol *typ);

#endif //CRCL_EMITTER_H
