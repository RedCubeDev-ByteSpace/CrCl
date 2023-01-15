//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_WHILESTATEMENT_H
#define CRCL_BOUND_WHILESTATEMENT_H

#include "../../boundnode.h"

typedef struct BOUND_WHILE_STATEMENT_NODE {
    BoundNode base;

    BoundNode *Condition;
    BoundNode *Body;
} BoundWhileStatementNode;

#endif //CRCL_BOUND_WHILESTATEMENT_H
