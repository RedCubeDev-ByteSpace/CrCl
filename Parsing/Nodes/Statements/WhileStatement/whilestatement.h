//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_WHILESTATEMENT_H
#define CRCL_WHILESTATEMENT_H

#include "../../node.h"

typedef struct WHILE_STATEMENT_NODE {
    Node base;

    Node *Condition;
    Node *Body;
} WhileStatementNode;

#endif //CRCL_WHILESTATEMENT_H
