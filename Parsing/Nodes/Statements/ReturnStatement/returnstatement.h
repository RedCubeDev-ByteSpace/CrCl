//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_RETURNSTATEMENT_H
#define CRCL_RETURNSTATEMENT_H

#include "../../node.h"

typedef struct RETURN_STATEMENT_NODE {
    Node base;

    Node *ReturnValue;
} ReturnStatementNode;

#endif //CRCL_RETURNSTATEMENT_H
