//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_LOCALVARIABLESYMBOL_H
#define CRCL_LOCALVARIABLESYMBOL_H

#include "symbol.h"
#include "typesymbol.h"
#include "variablesymbol.h"

typedef struct LOCAL_VARIABLE_SYMBOL LocalVariableSymbol;

struct LOCAL_VARIABLE_SYMBOL {
    VariableSymbol base;

    int UniqueId;
};

#endif //CRCL_LOCALVARIABLESYMBOL_H
