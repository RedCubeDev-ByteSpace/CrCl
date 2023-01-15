//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_SCOPE_H
#define CRCL_SCOPE_H

#include <stdbool.h>
#include "../Symbols/symbol.h"

typedef struct SCOPE Scope;

struct SCOPE {
    Symbol **SymbolBuffer;
    int BufferSize;
    int SymbolCount;

    Scope *Parent;
};

void AppendSymbol(Scope *scp, Symbol *sym);

bool TryRegisterSymbol(Scope *scp, Symbol *sym);
Symbol *TryLookupSymbol(Scope *scp, char *name);
Symbol *TryLookupLocalSymbol(Scope *scp, char *name);

#endif //CRCL_SCOPE_H
