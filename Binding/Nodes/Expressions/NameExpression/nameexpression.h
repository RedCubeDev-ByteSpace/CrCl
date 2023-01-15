//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_NAMEEXPRESSION_H
#define CRCL_BOUND_NAMEEXPRESSION_H

#include "../../boundnode.h"
#include "../../../Symbols/symbol.h"

typedef struct BOUND_NAME_EXPRESSION_NODE {
    BoundExpressionNode base;

    VariableSymbol *Variable;
} BoundNameExpressionNode;

#endif //CRCL_NAMEEXPRESSION_H
