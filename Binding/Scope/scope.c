//
// Created by redcube on 15/01/23.
//
#include <string.h>
#include "scope.h"

bool TryRegisterSymbol(Scope *scp, Symbol *sym) {
    // make sure we dont already have this symbol declared locally
    Symbol *lookup = TryLookupLocalSymbol(scp, sym->Name);

    // cant declare symbol, name is already taken in this scope
    if (lookup != NULL) return false;

    // ooooooootherwise -> declare this symbol
    AppendSymbol(scp, sym);
    return true;
}

// Will look for a symbol with the given name recursively up the chain of scopes
Symbol *TryLookupSymbol(Scope *scp, char *name) {
    // first look for this symbol locally
    Symbol *sym = TryLookupLocalSymbol(scp, name);

    // we found smth? -> cool beans
    if (sym != NULL) return sym;

    // oooootherwise, do we have a parent linked?
    if (scp->Parent != NULL)
        // delegate this lookup to it
        return TryLookupSymbol(scp->Parent, name);

    // if we dont -> sucks to be you i guess
    return NULL;
}

// Will only look for a symbol in the local symbol array and wont recurse
Symbol *TryLookupLocalSymbol(Scope *scp, char *name) {

    // loop through the symbol buffer
    for (int i = 0; i < scp->SymbolCount; i++) {
        Symbol *sym = scp->SymbolBuffer[i];

        // if we find a matching name -> cool beans
        if (strcmp(name, sym->Name) == 0)
            return sym;
    }

    // if we find nothin -> NUhL
    return NULL;
}