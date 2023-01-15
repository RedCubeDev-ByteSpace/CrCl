//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_PARAMETERSYMBOL_H
#define CRCL_PARAMETERSYMBOL_H

#include "symbol.h"
#include "typesymbol.h"
#include "variablesymbol.h"

typedef struct PARAMETER_SYMBOL ParameterSymbol;

struct PARAMETER_SYMBOL {
    VariableSymbol base;

    int UniqueId;
    int Ordinal;
};

#endif //CRCL_PARAMETERSYMBOL_H
