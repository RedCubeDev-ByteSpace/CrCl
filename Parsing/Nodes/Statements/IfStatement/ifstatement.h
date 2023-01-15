//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_IFSTATEMENT_H
#define CRCL_IFSTATEMENT_H

#include "../../node.h"

typedef struct IF_STATEMENT_NODE {
    Node base;

    Node *Condition;
    Node *ThenBlock;
    Node *ElseBlock;

} IfStatementNode;

#endif //CRCL_IFSTATEMENT_H
