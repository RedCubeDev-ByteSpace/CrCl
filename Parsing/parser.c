//
// Created by redcube on 14/01/23.
//
#include <stdlib.h>
#include <string.h>
#include <gc/gc.h>
#include <stdio.h>
#include "../Lexing/lexer.h"
#include "../Error/error.h"
#include "parser.h"
#include "Nodes/Statements/BlockStatement/blockstatement.h"
#include "Nodes/Statements/IfStatement/ifstatement.h"
#include "Nodes/Statements/LocalStatement/localstatement.h"
#include "Nodes/Statements/ReturnStatement/returnstatement.h"
#include "Nodes/Statements/ExpressionStatement/expressionstatement.h"
#include "Nodes/Expressions/LiteralExpression/literalexpression.h"
#include "Nodes/Expressions/AssignmentExpression/assignmentexpression.h"
#include "Nodes/Expressions/NameExpression/nameexpression.h"
#include "Nodes/Expressions/CallExpression/callexpression.h"
#include "Nodes/Expressions/BinaryExpression/binaryexpression.h"
#include "Nodes/Statements/WhileStatement/whilestatement.h"
#include "Nodes/Expressions/ParenthesizedExpression/parenthesizedexpression.h"

NodeList Parse(TokenList tokens) {
    Parser *prs = &(Parser) {
        tokens, 0, 0, 0, 0,
    };

    ParseMembers(prs);

    return (NodeList) {
        prs->NodeBuffer,
        prs->NodeCount
    };
}

// =====================================================================================================================
// Members
// =====================================================================================================================

void ParseMembers(Parser *prs) {
    // Memebors!!
    // this is stuff like
    // -> Functions
    // -> Globals
    // -> Structs
    // -> Enums
    // and uh thats it actually, i dont want any more for this one
    Token current;

    while ((current = CurrentToken(prs)).Type != Eof) {
        if (CurrentToken(prs).Type == FuncKeyword)
            ParseFunctionDeclaration(prs);
        else
            Die("Expected member, got depression instead. (%s)", TokenTypeNames[CurrentToken(prs).Type]);
    }
}

void ParseFunctionDeclaration(Parser *prs) {
    // func keyword
    Consume(prs, FuncKeyword);

    // function name
    Token identifier = Consume(prs, Identifier);

    Consume(prs, OpenParenthesis);  // (
    FunctionParameterList *params = ParseParameters(prs);
    Consume(prs, CloseParenthesis); // )

    // function return type
    TypeClause *returnType = NULL;

    if (CurrentToken(prs).Type == Identifier)
        returnType = ParseTypeClause(prs);

    BlockStatementNode *body = ParseBlockStatement(prs);

    FunctionMemberNode *mem = GC_MALLOC(sizeof(FunctionMemberNode));
    mem->base = (Node) {FunctionMember};
    mem->Identifier = identifier;
    mem->Parameters = params;
    mem->ReturnType = returnType;
    mem->Body       = body;

    AppendMember(prs, mem);
}

FunctionParameterList *ParseParameters(Parser *prs) {

    // somewhere to put our parameters
    FunctionParameter *parameters[64]; // max allowed params is 64
    int index = 0;

    // as long as we dont hit an Eof and get commas
    while (CurrentToken(prs).Type != Eof && CurrentToken(prs).Type != CloseParenthesis) {
        // parse a parameter
        parameters[index] = ParseParameter(prs);

        // check if we got a comma, if so we'll carry on parsing parameters
        if (CurrentToken(prs).Type != Comma)
            break;

        // coma :)
        Consume(prs, Comma);
    }

    // allocate some space for this and hand it back
    FunctionParameterList *me = GC_MALLOC(sizeof(FunctionParameterList));
    memcpy(me->Parameters, parameters, index * sizeof(FunctionParameterList*));
    me->Count = index;

    // don
    return me;
}

FunctionParameter *ParseParameter(Parser *prs) {
    // get the param name and type
    Token identifier = Consume(prs, Identifier);
    TypeClause *type = ParseTypeClause(prs);

    // allocate some space for this and hand it back
    FunctionParameter *me = GC_MALLOC(sizeof(FunctionParameter));
    me->Identifier = identifier;
    me->TypeClause = type;

    return me;
}

TypeClause *ParseTypeClause(Parser *prs) {
    Token typeName = Consume(prs, Identifier);

    TypeClause *subTypes[32]; // max 32 subtypes because i say so
    int index = 0;

    if (CurrentToken(prs).Type == OpenBracket) { // if [
        Consume(prs, OpenBracket); // [

        // recursively parse type clauses
        while (CurrentToken(prs).Type != Eof) {
            subTypes[index] = ParseTypeClause(prs);
            index++;

            if (CurrentToken(prs).Type != Comma)
                break;
            Consume(prs, Comma);
        }

        Consume(prs, CloseBracket); // ]
    }

    TypeClauseList *subClauses = NULL;

    // if we got subclauses
    if (index > 0) {
        // allocate some space for them
        subClauses = GC_MALLOC(sizeof(TypeClauseList));
        subClauses->Clauses = GC_MALLOC(index * sizeof(TypeClause*));

        // copy our clause pointers
        memcpy(subClauses->Clauses, subTypes, index * sizeof(TypeClause*));

        // remember how many we got
        subClauses->Count = index;
    }

    // allocate some space for this type clause
    TypeClause *me = GC_MALLOC(sizeof(TypeClause));
    me->Identifier = typeName;
    me->SubTypes = subClauses;

    return me;
}

// =====================================================================================================================
// Statements
// =====================================================================================================================

Node *ParseStatement(Parser *prs) {
    Node *stmt = NULL;

    switch (CurrentToken(prs).Type) {
        case OpenBrace:
            stmt = ParseBlockStatement(prs); break;
        case IfKeyword:
            stmt = ParseIfStatement(prs); break;
        case LocalKeyword:
            stmt = ParseLocalStatement(prs); break;
        case ReturnKeyword:
            stmt = ParseReturnStatement(prs); break;
        case WhileKeyword:
            stmt = ParseWhileStatement(prs); break;
        default:
            stmt = ParseExpressionStatement(prs); break;
            //Die("dude no fucking clue what to do with this %s", TokenTypeNames[CurrentToken(prs).Type]);
    }

    if (CurrentToken(prs).Type == Semicolon)
        Consume(prs, Semicolon);

    return stmt;
}

BlockStatementNode *ParseBlockStatement(Parser *prs) {
    Consume(prs, OpenBrace);

    Node *statements[512]; // hard limit on block statement length because fuck you
    int index = 0;

    while (CurrentToken(prs).Type != Eof && CurrentToken(prs).Type != CloseBrace) {
        statements[index] = ParseStatement(prs);
        index++;
    }

    Consume(prs, CloseBrace);

    // allocate some space for this block statement
    BlockStatementNode *me = GC_MALLOC(sizeof(BlockStatementNode));
    me->base.Type = BlockStatement;
    me->Statements = GC_MALLOC(sizeof(Node*) * index);
    memcpy(me->Statements, statements, sizeof(Node*) * index);

    return me;
}

IfStatementNode *ParseIfStatement(Parser *prs) {
    Consume(prs, IfKeyword);
    Node *condition = ParseExpression(prs);
    Node *thenStmt = ParseStatement(prs);
    Node *elseStmt = NULL;

    if (CurrentToken(prs).Type == ElseKeyword) {
        Consume(prs, ElseKeyword);
        elseStmt = ParseStatement(prs);
    }

    // allocate some space for this if statement
    IfStatementNode *me = GC_MALLOC(sizeof(IfStatementNode));
    me->base.Type = IfStatement;
    me->Condition = condition;
    me->ThenBlock = thenStmt;
    me->ElseBlock = elseStmt;

    return me;
}

LocalStatementNode *ParseLocalStatement(Parser *prs) {
    Consume(prs, LocalKeyword);
    TypeClause *type = NULL;

    if (PeekToken(prs, 1).Type != LeftArrow) {
        type = ParseTypeClause(prs);
    }

    Token identifier = Consume(prs, Identifier);

    Node *initializer = NULL;
    if (CurrentToken(prs).Type == LeftArrow) {
        Consume(prs, LeftArrow);
        initializer = ParseExpression(prs);
    }

    // allocate some space for this local statement
    LocalStatementNode *me = GC_MALLOC(sizeof(LocalStatementNode));
    me->base.Type = VariableDeclarationStatement;
    me->Identifier = identifier;
    me->Type = type;
    me->Initializer = initializer;

    return me;
}

ReturnStatementNode *ParseReturnStatement(Parser *prs) {
    Consume(prs, ReturnKeyword);
    Node *returnValue = NULL;

    if (CurrentToken(prs).Type != Semicolon)
        returnValue = ParseExpression(prs);

    // allocate some space for this return statement
    ReturnStatementNode *me = GC_MALLOC(sizeof(ReturnStatementNode));
    me->base.Type = ReturnStatement;
    me->ReturnValue = returnValue;

    return me;
}

WhileStatementNode *ParseWhileStatement(Parser *prs) {
    Consume(prs, WhileKeyword);
    Node *condition = ParseExpression(prs);
    Node *body = ParseStatement(prs);

    // allocate some space for this return statement
    WhileStatementNode *me = GC_MALLOC(sizeof(WhileStatementNode));
    me->base.Type = WhileStatement;
    me->Condition = condition;
    me->Body = body;

    return me;
}

ExpressionStatementNode *ParseExpressionStatement(Parser *prs) {
    Node *expression = ParseExpression(prs);

    // allocate some space for this expression statement
    ExpressionStatementNode *me = GC_MALLOC(sizeof(ExpressionStatementNode));
    me->base.Type = ExpressionStatement;
    me->Expression = expression;

    return me;
}

// =====================================================================================================================
// Expressions
// =====================================================================================================================

Node *ParseExpression(Parser *prs) {
    return ParseBinaryExpression(prs, 0);
}

Node *ParseBinaryExpression(Parser *prs, int precedence) {
    Node *left = ParsePrimaryExpression(prs);
    int myPrecedence;

    while ((myPrecedence = BinaryOperatorPrecedence(CurrentToken(prs))) >= precedence && myPrecedence != 0) {
        BinaryOperator op = ParseBinaryOperator(prs);
        Node *right = ParseBinaryExpression(prs, myPrecedence);

        // allocate some space for this binary expression
        BinaryExpressionNode *me = GC_MALLOC(sizeof(BinaryExpressionNode));
        me->base.Type = BinaryExpression;
        me->Left = left;
        me->Right = right;
        me->Operator = op;

        left = me;
    }

    return left;
}

BinaryOperator ParseBinaryOperator(Parser *prs) {
    switch (CurrentToken(prs).Type) {
        case Star: Consume(prs, Star); return MultiplyOperator;
        case Slash: Consume(prs, Slash); return DivideOperator;
        case Plus: Consume(prs, Plus); return PlusOperator;
        case Minus: Consume(prs, Minus); return MinusOperator;
        case Equals: Consume(prs, Equals); return EqualOperator;
        case Unequals: Consume(prs, Unequals); return UnequalOperator;
        case LessThan: Consume(prs, LessThan); return LessThanOperator;
        case LessEqual: Consume(prs, LessEqual); return LessEqualsOperator;
        case GreaterThan: Consume(prs, GreaterThan); return GreaterThanOperator;
        case GreaterEqual: Consume(prs, GreaterEqual); return GreaterEqualsOperator;
        case And: Consume(prs, And); return AndOperator;
        case AndAnd: Consume(prs, AndAnd); return BitwiseAndOperator;
        case Pipe: Consume(prs, Pipe); return OrOperator;
        case PipePipe: Consume(prs, PipePipe); return BitwiseOrOperator;

        default:
            Die("damn i dont know this operator (%s)", TokenTypeNames[CurrentToken(prs).Type]);
            return 0;
    }
}

Node *ParsePrimaryExpression(Parser *prs) {
    switch (CurrentToken(prs).Type) {
        case OpenParenthesis:
            return ParseParenthesizedExpression(prs);

        case Number:
        case String:
        case NullKeyword:
            return ParseLiteralExpression(prs);

        case Identifier:
            if (PeekToken(prs, 1).Type == LeftArrow)
                return ParseAssignmentExpression(prs);

            if (PeekToken(prs, 1).Type == OpenParenthesis)
                return ParseCallExpression(prs);

            return ParseNameExpression(prs);

        default:
            Die("Damn dont know what this expression is supposed to be (token: %s, %s)", TokenTypeNames[CurrentToken(prs).Type], CurrentToken(prs).Text);
    }
}

ParenthesizedExpressionNode *ParseParenthesizedExpression(Parser *prs) {
    Consume(prs, OpenParenthesis);
    Node *expression = ParseExpression(prs);
    Consume(prs, CloseParenthesis);

    // allocate some space for this () expression
    ParenthesizedExpressionNode *me = GC_MALLOC(sizeof(ParenthesizedExpressionNode));
    me->base.Type = ParenthesizedExpression;
    me->Expression = expression;

    return me;
}

LiteralExpressionNode *ParseLiteralExpression(Parser *prs) {
    // allocate some space for this literal expression
    LiteralExpressionNode *me = GC_MALLOC(sizeof(LiteralExpressionNode));
    me->base.Type = LiteralExpression;
    me->Literal = CurrentToken(prs);

    prs->CurrentPointer++;

    return me;
}

AssignmentExpressionNode *ParseAssignmentExpression(Parser *prs) {
    Token identifier = Consume(prs, Identifier);
    Consume(prs, LeftArrow);
    Node *value = ParseExpression(prs);

    // allocate some space for this assignment expression
    AssignmentExpressionNode *me = GC_MALLOC(sizeof(AssignmentExpressionNode));
    me->base.Type = LiteralExpression;
    me->Identifier = identifier;
    me->Value = value;

    return me;
}

CallExpressionNode *ParseCallExpression(Parser *prs) {
    Token identifier = Consume(prs, Identifier);
    Consume(prs, OpenParenthesis);

    Node *arguments[64]; // among us
    int index = 0;

    while (CurrentToken(prs).Type != Eof && CurrentToken(prs).Type != CloseParenthesis) {
        arguments[index] = ParseExpression(prs);
        index++;

        if (CurrentToken(prs).Type != Comma)
            break;
        Consume(prs, Comma);
    }

    Consume(prs, CloseParenthesis);

    NodeList args;
    args.NodeBuffer = GC_MALLOC(sizeof(Node*) * index);
    memcpy(args.NodeBuffer, arguments, sizeof(Node*) * index);
    args.Count = index;

    // allocate some space for this call expression
    CallExpressionNode *me = GC_MALLOC(sizeof(CallExpressionNode));
    me->base.Type = CallExpression;
    me->Identifier = identifier;
    me->Arguments = args;

    return me;
}

NameExpressionNode *ParseNameExpression(Parser *prs) {
    Token identifier = Consume(prs, Identifier);

    // allocate some space for this name expression
    NameExpressionNode *me = GC_MALLOC(sizeof(NameExpressionNode));
    me->base.Type = NameExpression;
    me->Identifier = identifier;

    return me;
}