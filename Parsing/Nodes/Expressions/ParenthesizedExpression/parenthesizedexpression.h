//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_PARENTHESIZEDEXPRESSION_H
#define CRCL_PARENTHESIZEDEXPRESSION_H

#include "../../node.h"

typedef struct PARENTHESIZED_EXPRESSION_NODE {
    Node base;

    Node *Expression;
} ParenthesizedExpressionNode;

#endif //CRCL_PARENTHESIZEDEXPRESSION_H
