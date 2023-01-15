//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_LITERALEXPRESSION_H
#define CRCL_BOUND_LITERALEXPRESSION_H

#include "../../boundnode.h"

typedef struct BOUND_LITERAL_EXPRESSION_NODE {
    BoundExpressionNode base;
    void *Value;
} BoundLiteralExpressionNode;

#endif //CRCL_LITERALEXPRESSION_H
