//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_RETURNSTATEMENT_H
#define CRCL_BOUND_RETURNSTATEMENT_H

#include "../../boundnode.h"

typedef struct BOUND_RETURN_STATEMENT_NODE {
    BoundNode base;

    BoundNode *ReturnValue;
} BoundReturnStatementNode;

#endif //CRCL_BOUND_RETURNSTATEMENT_H
