//
// Created by redcube on 14/01/23.
//

#ifndef CRCL_TYPECLAUSE_H
#define CRCL_TYPECLAUSE_H

#include "../../Lexing/lexer.h"

typedef struct TYPE_CLAUSE TypeClause;
typedef struct TYPE_CLAUSE_LIST TypeClauseList;

// the not-quite node
struct TYPE_CLAUSE {
    Token Identifier;
    TypeClauseList *SubTypes;
};

struct TYPE_CLAUSE_LIST {
    TypeClause *Clauses;
    int Count;
};

#endif //CRCL_TYPECLAUSE_H
