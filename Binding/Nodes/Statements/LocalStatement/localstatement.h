//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_LOCALSTATEMENT_H
#define CRCL_BOUND_LOCALSTATEMENT_H

#include "../../boundnode.h"
#include "../../../Symbols/localvariablesymbol.h"

typedef struct BOUND_LOCAL_STATEMENT_NODE {
    BoundNode base;

    LocalVariableSymbol *Variable;
    BoundExpressionNode *Initializer;

} BoundLocalStatementNode;

#endif //CRCL_BOUND_LOCALSTATEMENT_H
