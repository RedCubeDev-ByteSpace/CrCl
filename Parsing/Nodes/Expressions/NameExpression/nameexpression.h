//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_NAMEEXPRESSION_H
#define CRCL_NAMEEXPRESSION_H

#include "../../node.h"

typedef struct NAME_EXPRESSION_NODE {
    Node base;

    Token Identifier;
} NameExpressionNode;

#endif //CRCL_NAMEEXPRESSION_H
