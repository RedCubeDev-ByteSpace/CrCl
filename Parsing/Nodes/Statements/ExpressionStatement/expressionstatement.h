//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_EXPRESSIONSTATEMENT_H
#define CRCL_EXPRESSIONSTATEMENT_H

#include "../../node.h"

typedef struct EXPRESSION_STATEMENT_NODE {
    Node base;

    Node *Expression;
} ExpressionStatementNode;

#endif //CRCL_EXPRESSIONSTATEMENT_H
