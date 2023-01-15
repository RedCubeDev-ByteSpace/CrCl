//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_LOCALSTATEMENT_H
#define CRCL_LOCALSTATEMENT_H

#include "../../node.h"
#include "../../typeclause.h"

typedef struct LOCAL_STATEMENT_NODE {
    Node base;

    Token Identifier;
    TypeClause *Type;
    Node *Initializer;

} LocalStatementNode;

#endif //CRCL_LOCALSTATEMENT_H
