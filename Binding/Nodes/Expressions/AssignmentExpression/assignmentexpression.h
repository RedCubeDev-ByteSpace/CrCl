//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_ASSIGNMENTEXPRESSION_H
#define CRCL_BOUND_ASSIGNMENTEXPRESSION_H

#include "../../boundnode.h"
#include "../../../Symbols/symbol.h"
#include "../../../Symbols/variablesymbol.h"

typedef struct BOUND_ASSINGMENT_EXPRESSION_NODE {
    BoundExpressionNode base;

    VariableSymbol *Variable;
    BoundExpressionNode *Value;
} BoundAssignmentExpressionNode;

#endif //CRCL_BOUND_ASSIGNMENTEXPRESSION_H
