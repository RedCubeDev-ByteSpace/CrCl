//
// Created by redcube on 15/01/23.
//
#include <string.h>
#include "typesymbol.h"

bool typcmp(TypeSymbol *a, TypeSymbol *b) {
    // names differ
    if (strcmp(a->base.Name, b->base.Name) != 0) return false;

    // subtypes differ
    if (a->Subtypes.Count != b->Subtypes.Count) return false;

    // go through all subtypes
    for (int i = 0; i < a->Subtypes.Count; i++)
        // check if theyre equal
        if (!typcmp(a->Subtypes.Symbols[i], b->Subtypes.Symbols[i])) return false;

    // ok
    return true;
}