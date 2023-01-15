//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_TYPESYMBOL_H
#define CRCL_TYPESYMBOL_H

#include <stdbool.h>
#include "symbol.h"

typedef struct TYPE_SYMBOL TypeSymbol;
typedef struct TYPE_SYMBOL_LIST TypeSymbolList;

struct TYPE_SYMBOL_LIST {
    TypeSymbol **Symbols;
    int Count;
};

struct TYPE_SYMBOL {
    Symbol base;
    TypeSymbolList Subtypes;
};

bool typcmp(TypeSymbol *a, TypeSymbol *b);

#endif //CRCL_TYPESYMBOL_H
