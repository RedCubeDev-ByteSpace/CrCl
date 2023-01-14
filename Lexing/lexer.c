//
// Created by redcube on 14/01/23.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "../Error/error.h"

// ============================================================================
// Lexing
// ============================================================================

TokenList Lex(char *source) {
    // create new lexer
    Lexer *lxr = &(Lexer) {
      source,
      0,
      strlen(source),
      0, 0
    };

    while (CurrentChar(lxr) != 0) {
        char current = CurrentChar(lxr);

        // keywords and identifiers
        if (isalpha(current))
            LexKeyword(lxr);

        // numbers (only ints atm)
        else if (isdigit(current))
            LexNumber(lxr);

        // strinks
        else if (current == '"')
            LexString(lxr);

        // comments
        else if (current == '/' && PeekChar(lxr, 1) == '/')
            LexComment(lxr);

        // other symbols
        else
            LexSymbol(lxr);
    }

    return (TokenList) {
        lxr->TokenBuffer,
        lxr->TokenCount
    };
}

void LexKeyword(Lexer *lxr) {
    char current = 0;
    char buffer[256]; // i don't think ill need more
    int  index = 0;

    // while not EOF and is current is letter
    while ((current = CurrentChar(lxr)) != 0 && index < 255 &&
            (isalpha(current) || isdigit(current))) {

        buffer[index] = current;

        // step
        lxr->CurrentPointer++;
        index++;
    }
    buffer[index] = 0;

    // copy the text out from the buffer
    char *text = malloc(index);
    memcpy(text, buffer, index+1);

    TokenType type = ClassifyKeyword(text);

    AppendToken(lxr, (Token) {
        type, text, NULL
    });
}

void LexNumber(Lexer *lxr) {
    char current;
    char buffer[256]; // i don't think ill need more
    int  index = 0;

    // while not EOF and is current is letter
    while ((current = CurrentChar(lxr)) != 0 && index < 255 &&
            isdigit(current)) {

        buffer[index] = current;

        // step
        lxr->CurrentPointer++;
        index++;
    }

    // casual null termination
    buffer[index] = 0;

    // store the text in case we need it later
    char *text = malloc(index);
    memcpy(text, buffer, index+1);

    int value = atoi(buffer); // living dangerously

    // store the number somewhere
    int *val = malloc(sizeof(int));
    *val = value;

    // we done
    AppendToken(lxr, (Token) {
       Number, text, val
    });
}

void LexString(Lexer *lxr) {
    // some quirky locals
    char current = 0;
    char buffer[512]; // mmmmmmm no clue how big this max should be
    int  index = 0;

    // skip the leading "
    buffer[0] = '"';
    index++;
    lxr->CurrentPointer++;

    // while not EOF and current isnt "
    while ((current = CurrentChar(lxr)) != 0 && current != '"') {

        if (index >= 512) {
            Die("Damn looks like your string is too long. Sucks to be you i guess lmao");
        }

        buffer[index] = current;

        // step
        lxr->CurrentPointer++;
        index++;
    }

    // skip the tailing "
    buffer[index] = CurrentChar(lxr);
    index++;
    lxr->CurrentPointer++;

    // chad null termination
    buffer[index] = 0;

    // copy the text out from the buffer
    char *text = malloc(index);
    memcpy(text, buffer, index+1);

    // copy the text without the quotation marks
    char *value = malloc(index-2);
    value[index-2] = 0;
    memcpy(value, &(buffer[1]), index-2);

    AppendToken(lxr, (Token) {
            String, text, value
    });
}

void LexComment(Lexer *lxr) {
    char current;

    // skip forward until we hit the EOF or a new line
    while ((current = CurrentChar(lxr)) != 0 && current != '\n') lxr->CurrentPointer++;
}

void LexSymbol(Lexer *lxr) {
    TokenType type  = -1;
    int tokenLength = 1;

    switch (CurrentChar(lxr)) {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            lxr->CurrentPointer++;
            return;

        case ';': type = Semicolon; break;
        case '(': type = OpenParenthesis; break;
        case ')': type = CloseParenthesis; break;
        case '{': type = OpenBrace; break;
        case '}': type = CloseBrace; break;
        case '[': type = OpenBracket; break;
        case ']': type = CloseBracket; break;
        case '<':
            if (PeekChar(lxr, 1) == '-') {
                type = LeftArrow;
                tokenLength = 2;
                break;
            }
            else if (PeekChar(lxr, 1) == '=') {
                type = LessEqual;
                tokenLength = 2;
                break;
            }

            type = LessThan;
            break;

        case '=': type = Equals; break;
        case '!':
            if (PeekChar(lxr, 1) == '=') {
                type = Unequals;
                tokenLength = 2;
                break;
            }

            type = Not;
            break;
        case '>':
            if (PeekChar(lxr, 1) == '=') {
                type = GreaterEqual;
                tokenLength = 2;
                break;
            }

            type = GreaterThan;
            break;
        case '|':
            if (PeekChar(lxr, 1) == '|') {
                type = PipePipe;
                tokenLength = 2;
                break;
            }

            type = Pipe;
            break;
        case '&':
            if (PeekChar(lxr, 1) == '&') {
                type = AndAnd;
                tokenLength = 2;
                break;
            }

            type = And;
            break;
        case '+': type = Plus; break;
        case '-':
            if (PeekChar(lxr, 1) == '>') {
                type = RightArrow;
                tokenLength = 2;
                break;
            }

            type = Minus;
            break;
        case '/': type = Slash; break;
        case '*': type = Star; break;
    }

    // we found a character we dont know
    if (type == -1) {
        char current = CurrentChar(lxr);
        Die("Unexpected character %c (%d)", current, current);
    }

    // store the text for some time later
    char *text = malloc(tokenLength + 1);
    text[tokenLength] = 0;
    memcpy(text, &(lxr->SourceCode[lxr->CurrentPointer]), tokenLength);

    // step the char index
    lxr->CurrentPointer += tokenLength;

    // append the token to the list
    AppendToken(lxr, (Token) {
       type, text, NULL
    });
}

// ---------------------------------------------------------------------------------------------------------------------

TokenType ClassifyKeyword(char *text) {

    // is this a keyword?
    if (strcmp(text, "func") == 0)
        return FuncKeyword;
    else if (strcmp(text, "local") == 0)
        return LocalKeyword;
    else if (strcmp(text, "global") == 0)
        return GlobalKeyword;
    else if (strcmp(text, "return") == 0)
        return ReturnKeyword;
    else if (strcmp(text, "NULL") == 0)
        return NullKeyword;

    // if we dont find a matching one -> its an identifier
    else
        return Identifier;
}