//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_VARIABLESYMBOL_H
#define CRCL_VARIABLESYMBOL_H

#include "symbol.h"
#include "typesymbol.h"

typedef struct VARIABLE_SYMBOL VariableSymbol;

struct VARIABLE_SYMBOL {
    Symbol base;
    TypeSymbol *Type;
};

#endif //CRCL_VARIABLESYMBOL_H
