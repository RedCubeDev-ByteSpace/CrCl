//
// Created by redcube on 16/01/23.
//

#ifndef CRCL_EXTERNAL_H
#define CRCL_EXTERNAL_H

#include "../../node.h"
#include "../../../../Lexing/lexer.h"
#include "../FunctionMember/function.h"

typedef struct EXTERNAL_FUNCTION_MEMBER_NODE {
    Node base;

    Token Identifier;
    FunctionParameterList *Parameters;
    TypeClause *ReturnType;
    bool IsVariadic;

} ExternalFunctionMemberNode;

#endif //CRCL_EXTERNAL_H
