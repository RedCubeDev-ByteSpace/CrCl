//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUND_BLOCKSTATEMENT_H
#define CRCL_BOUND_BLOCKSTATEMENT_H

#include "../../boundnode.h"

typedef struct BOUND_STATEMENT_LIST {
    BoundNode **Statements;
    int Count;
} BoundStatementList;

typedef struct BOUND_BLOCK_STATEMENT_NODE {
    BoundNode base;

    BoundStatementList Statements;

} BoundBlockStatementNode;

#endif //CRCL_BOUND_BLOCKSTATEMENT_H
