#include <gc/gc.h>
#include <string.h>
#include "scope.h"
#include "../../Lexing/lexer.h"

//
// Created by redcube on 15/01/23.
//
void AppendSymbol(Scope *scp, Symbol *sym) {
    // no buffer?
    if (scp->SymbolBuffer == NULL) {
        scp->SymbolBuffer = GC_MALLOC(BUFFER_GROWTH_FACTOR * sizeof(Symbol*));
        scp->BufferSize  = BUFFER_GROWTH_FACTOR;
    }

    // no buffer space left?
    if (scp->SymbolCount >= scp->BufferSize) {
        // grow it!
        int newBufferSize = scp->BufferSize + BUFFER_GROWTH_FACTOR;
        size_t newBufferSizeInBytes = newBufferSize * sizeof(Symbol*);

        // try reallocating first as its way faster than copying
        Symbol **newBuffer = GC_REALLOC(scp->SymbolBuffer, newBufferSizeInBytes);

        // reallocation failed!
        if (newBuffer == NULL) {
            // GC_MALLOC and copy manually (slow)
            newBuffer = GC_MALLOC(newBufferSizeInBytes);
            memcpy(newBuffer, scp->SymbolBuffer, newBufferSizeInBytes);
        }

        // update lexer object
        scp->SymbolBuffer = newBuffer;
        scp->BufferSize  = newBufferSize;
    }

    // add the new token
    scp->SymbolBuffer[scp->SymbolCount] = sym;
    scp->SymbolCount++;
}