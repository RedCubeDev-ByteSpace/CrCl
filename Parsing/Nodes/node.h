//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_NODE_H
#define CRCL_NODE_H

#include "../parser.h"

typedef struct NODE Node;
typedef struct NODE_LIST NodeList;

// Node Object (base class)
struct NODE {
    int Type;
};

struct NODE_LIST {
    Node **NodeBuffer;
    int Count;
};

#endif //CRCL_NODE_H
