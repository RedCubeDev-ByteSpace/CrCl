//
// Created by redcube on 14/01/23.
//

#ifndef CRCL_FUNCTION_H
#define CRCL_FUNCTION_H

#include "../../node.h"
#include "../../typeclause.h"
#include "../../Statements/BlockStatement/blockstatement.h"


typedef struct FUNCTION_PARAMETER {
    Token Identifier;
    TypeClause *TypeClause;
} FunctionParameter;

typedef struct FUNCTION_PARAMETER_LIST {
    FunctionParameter **Parameters;
    int Count;
} FunctionParameterList;

typedef struct FUNCTION_MEMBER_NODE {
    Node base;

    Token Identifier;
    FunctionParameterList *Parameters;
    TypeClause *ReturnType;

    BlockStatementNode *Body;

} FunctionMemberNode;

#endif //CRCL_FUNCTION_H
