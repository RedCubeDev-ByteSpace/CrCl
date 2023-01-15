//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_BINARYEXPRESSION_H
#define CRCL_BOUND_BINARYEXPRESSION_H

#include "../../boundnode.h"
#include "../../../Symbols/typesymbol.h"
#include "../../../../Parsing/Nodes/Expressions/BinaryExpression/binaryexpression.h"

typedef struct BOUND_BINARY_OPERATOR {
    BinaryOperator Operator;
    TypeSymbol *Left;
    TypeSymbol *Right;
    TypeSymbol *Result;
} BoundBinaryOperator;

typedef struct BOUND_BINARY_EXPRESSION_NODE {
    BoundExpressionNode base;

    BoundBinaryOperator Operator;
    BoundExpressionNode *Left;
    BoundExpressionNode *Right;
} BoundBinaryExpressionNode;

void InitOperators();
BoundBinaryOperator *BindBinaryOperator(BinaryOperator op, TypeSymbol *left, TypeSymbol *right);

#endif //CRCL_BINARYEXPRESSION_H
