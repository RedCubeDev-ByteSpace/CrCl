//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BOUNDNODE_H
#define CRCL_BOUNDNODE_H

#include "../Symbols/typesymbol.h"

typedef struct BOUND_NODE BoundNode;
typedef struct BOUND_NODE_LIST BoundNodeList;
typedef struct BOUND_EXPRESSION_NODE BoundExpressionNode;

// Node Object (base class)
struct BOUND_NODE {
    int Type;
};

struct BOUND_NODE_LIST {
    BoundNode **NodeBuffer;
    int Count;
};

struct BOUND_EXPRESSION_NODE {
    BoundNode base;
    TypeSymbol *Type;
};

#endif //CRCL_BOUNDNODE_H
