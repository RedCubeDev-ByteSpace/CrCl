//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_IFSTATEMENT_H
#define CRCL_BOUND_IFSTATEMENT_H

#include "../../boundnode.h"

typedef struct BOUND_IF_STATEMENT_NODE {
    BoundNode base;

    BoundNode *Condition;
    BoundNode *ThenBlock;
    BoundNode *ElseBlock;

} BoundIfStatementNode;

#endif //CRCL_BOUND_IFSTATEMENT_H
