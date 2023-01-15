//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_FUNCTIONSYMBOL_H
#define CRCL_FUNCTIONSYMBOL_H

#include "symbol.h"
#include "typesymbol.h"

typedef struct FUNCTION_SYMBOL {
    Symbol base;

    TypeSymbol *ReturnType;
    SymbolList Parameters;
} FunctionSymbol;

#endif //CRCL_FUNCTIONSYMBOL_H
