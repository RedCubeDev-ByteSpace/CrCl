//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_LITERALEXPRESSION_H
#define CRCL_LITERALEXPRESSION_H

#include "../../node.h"

typedef struct LITERAL_EXPRESSION_NODE {
    Node base;
    Token Literal;
} LiteralExpressionNode;

#endif //CRCL_LITERALEXPRESSION_H
