//
// Created by redcube on 14/01/23.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gc.h>
#include "../Error/error.h"
#include "lexer.h"

char* TokenTypeNames[] = {
        "Eof",

        // Symbols
        "Semicolon",        // ;
        "Comma",            // ,
        "OpenParenthesis",  // (
        "CloseParenthesis", // )
        "OpenBrace",        // {
        "CloseBrace",       // }
        "OpenBracket",      // [
        "CloseBracket",     // ]
        "LeftArrow",        // <-
        "RightArrow",       // ->
        "Collector",        // ~
        "CollectorArrow",   // ~>
        "Equals",           // =
        "Unequals",         // !=
        "GreaterThan",      // >
        "LessThan",         // <
        "GreaterEqual",     // >=
        "LessEqual",        // <=
        "Not",              // !
        "Pipe",             // |
        "PipePipe",         // ||
        "And",              // &
        "AndAnd",           // &&
        "Plus",             // +
        "Minus",            // -
        "Slash",            // /
        "Star",             // *

        // Literals
        "String",           // "..."
        "Number",           // 100

        // Keywords
        "FuncKeyword",
        "ExtKeyword",
        "LocalKeyword",
        "GlobalKeyword",
        "ReturnKeyword",
        "IfKeyword",
        "ElseKeyword",
        "WhileKeyword",
        "BreakKeyword",
        "ContinueKeyword",
        "NullKeyword",

        // Identifiers
        "Identifier",
};

// = ObjHelpers ===============================================================
void DeleteToken(Token tok) {
    GC_FREE(tok.Text);
    GC_FREE(tok.Value);
}

// ============================================================================
// Helpers
// ============================================================================
void AppendToken(Lexer *lxr, Token tok) {


    // no buffer?
    if (lxr->TokenBuffer == NULL) {
        lxr->TokenBuffer = GC_MALLOC(BUFFER_GROWTH_FACTOR * sizeof(Token));
        lxr->BufferSize  = BUFFER_GROWTH_FACTOR;
    }

    // no buffer space left?
    if (lxr->TokenCount >= lxr->BufferSize) {
        // grow it!
        int newBufferSize = lxr->BufferSize + BUFFER_GROWTH_FACTOR;
        size_t newBufferSizeInBytes = newBufferSize * sizeof(Token);

        // try reallocating first as its way faster than copying
        Token *newBuffer = GC_REALLOC(lxr->TokenBuffer, newBufferSizeInBytes);

        // reallocation failed!
        if (newBuffer == NULL) {
            // GC_MALLOC and copy manually (slow)
            newBuffer = GC_MALLOC(newBufferSizeInBytes);
            memcpy(newBuffer, lxr->TokenBuffer, newBufferSizeInBytes);
        }

        // update lexer object
        lxr->TokenBuffer = newBuffer;
        lxr->BufferSize  = newBufferSize;
    }

    // add the new token
    lxr->TokenBuffer[lxr->TokenCount] = tok;
    lxr->TokenCount++;
}

char CurrentChar(Lexer *lxr) {
    PeekChar(lxr, 0);
}

char PeekChar(Lexer *lxr, int offset) {
    if (lxr->CurrentPointer + offset >= lxr->SourceLength) return 0;
    return lxr->SourceCode[lxr->CurrentPointer + offset];
}

void PrintTokenList(TokenList lst) {
    for (int i = 0; i < lst.Count; i++) {
        Token token = lst.Tokens[i];

        char *col;
        if (token.Type > 0 && token.Type <= Star)
            col = KCYN;
        else if (token.Type == String || token.Type == Number)
            col = KGRN;
        else if (token.Type == Identifier)
            col = KYEL;
        else if (token.Type >= FuncKeyword)
            col = KMAG;

        printf("%stoken %-20s [Type %s]\n", col, token.Text, TokenTypeNames[token.Type]);

        if (token.Type == String)
            printf(" └> Value: %s\n", (char*)token.Value);
        else if (token.Type == Number)
            printf(" └> Value: %d\n", *((int*)token.Value));
    }

    printf(KNRM);
}