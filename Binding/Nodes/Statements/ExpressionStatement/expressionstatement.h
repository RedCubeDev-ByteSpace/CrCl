//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_EXPRESSIONSTATEMENT_H
#define CRCL_BOUND_EXPRESSIONSTATEMENT_H

#include "../../boundnode.h"

typedef struct BOUND_EXPRESSION_STATEMENT_NODE {
    BoundNode base;

    BoundExpressionNode *Expression;
} BoundExpressionStatementNode;

#endif //CRCL_BOUND_EXPRESSIONSTATEMENT_H
