//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BLOCKSTATEMENT_H
#define CRCL_BLOCKSTATEMENT_H

#include "../../../parser.h"

typedef struct STATEMENT_LIST {
    Node **Statements;
    int Count;
} StatementList;

typedef struct BLOCK_STATEMENT_NODE {
    Node base;

    StatementList *Statements;

} BlockStatementNode;

#endif //CRCL_BLOCKSTATEMENT_H
