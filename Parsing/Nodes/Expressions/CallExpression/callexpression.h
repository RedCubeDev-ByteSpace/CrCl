//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_CALLEXPRESSION_H
#define CRCL_CALLEXPRESSION_H

#include "../../node.h"

typedef struct CALL_EXPRESSION_NODE {
    Node base;

    Token Identifier;
    NodeList Arguments;
} CallExpressionNode;

#endif //CRCL_CALLEXPRESSION_H
