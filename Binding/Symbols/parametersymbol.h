//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_PARAMETERSYMBOL_H
#define CRCL_PARAMETERSYMBOL_H

#include "symbol.h"
#include "typesymbol.h"

typedef struct PARAMETER_SYMBOL ParameterSymbol;

struct PARAMETER_SYMBOL {
    Symbol base;

    int UniqueId;
    int Ordinal;
    TypeSymbol *Type;
};

#endif //CRCL_PARAMETERSYMBOL_H
