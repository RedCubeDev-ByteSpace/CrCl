//
// Created by redcube on 14/01/23.
//
#include <gc/gc.h>
#include <string.h>
#include "parser.h"
#include "../Error/error.h"

Token CurrentToken(Parser *prs) {
    return PeekToken(prs, 0);
}

Token PeekToken(Parser *prs, int offset) {
    // make sure we aren't out of range
    if (prs->CurrentPointer + offset >= prs->Tokens.Count)
        return (Token) { Eof };

    // return the token at the current position + the given offset
    return prs->Tokens.Tokens[prs->CurrentPointer + offset];
}

Token Consume(Parser *prs, TokenType type) {
    Token current = CurrentToken(prs);

    // mmmmmmmm death
    if (current.Type != type)
        Die("Expected token of type %s, got %s instead (%d, %d) (%s)", TokenTypeNames[type], TokenTypeNames[current.Type], type, current.Type, current.Text);

    // okay cool, move on and return the token
    prs->CurrentPointer++;
    return PeekToken(prs, -1);
}

void AppendMember(Parser *prs, Node *node) {
    // no buffer?
    if (prs->NodeBuffer == NULL) {
        prs->NodeBuffer = GC_MALLOC(BUFFER_GROWTH_FACTOR * sizeof(Node*));
        prs->BufferSize  = BUFFER_GROWTH_FACTOR;
    }

    // no buffer space left?
    if (prs->NodeCount >= prs->BufferSize) {
        // grow it!
        int newBufferSize = prs->BufferSize + BUFFER_GROWTH_FACTOR;
        size_t newBufferSizeInBytes = newBufferSize * sizeof(Node*);

        // try reallocating first as its way faster than copying
        Node **newBuffer = GC_REALLOC(prs->NodeBuffer, newBufferSizeInBytes);

        // reallocation failed!
        if (newBuffer == NULL) {
            // GC_MALLOC and copy manually (slow)
            newBuffer = GC_MALLOC(newBufferSizeInBytes);
            memcpy(newBuffer, prs->NodeBuffer, newBufferSizeInBytes);
        }

        // update lexer object
        prs->NodeBuffer = newBuffer;
        prs->BufferSize  = newBufferSize;
    }

    // add the new token
    prs->NodeBuffer[prs->NodeCount] = node;
    prs->NodeCount++;
}