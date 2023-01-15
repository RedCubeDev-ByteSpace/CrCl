//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_CALLEXPRESSION_H
#define CRCL_BOUND_CALLEXPRESSION_H

#include "../../boundnode.h"
#include "../../../Symbols/functionsymbol.h"

typedef struct BOUND_CALL_EXPRESSION_NODE {
    BoundExpressionNode base;

    FunctionSymbol *Receiver;
    BoundNodeList Args;
} BoundCallExpressionNode;

#endif //CRCL_CALLEXPRESSION_H
