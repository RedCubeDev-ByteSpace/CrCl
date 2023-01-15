//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_ASSIGNMENTEXPRESSION_H
#define CRCL_ASSIGNMENTEXPRESSION_H

#include "../../node.h"

typedef struct ASSINGMENT_EXPRESSION_NODE {
    Node base;

    Token Identifier;
    Node *Value;
} AssignmentExpressionNode;

#endif //CRCL_ASSIGNMENTEXPRESSION_H
