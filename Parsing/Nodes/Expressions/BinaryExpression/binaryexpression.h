//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BINARYEXPRESSION_H
#define CRCL_BINARYEXPRESSION_H

#include "../../node.h"

typedef enum BINARY_OPERATOR {
    PlusOperator,
    MinusOperator,
    MultiplyOperator,
    DivideOperator,
    EqualOperator,
    UnequalOperator,
    GreaterThanOperator,
    GreaterEqualsOperator,
    LessThanOperator,
    LessEqualsOperator,
    AndOperator,
    BitwiseAndOperator,
    OrOperator,
    BitwiseOrOperator,
} BinaryOperator;

typedef struct BINARY_EXPRESSION_NODE {
    Node base;

    BinaryOperator Operator;
    Node *Left;
    Node *Right;
} BinaryExpressionNode;

int BinaryOperatorPrecedence(Token tok);

#endif //CRCL_BINARYEXPRESSION_H
