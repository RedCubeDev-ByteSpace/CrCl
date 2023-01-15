//
// Created by redcube on 14/01/23.
//

#ifndef CRCL_LEXER_H
#define CRCL_LEXER_H

// ============================================================================
// ENUMS
// ============================================================================
typedef enum TOKEN_TYPE {
    Eof,

    // Symbols
    Semicolon,        // ;
    Comma,            // ,
    OpenParenthesis,  // (
    CloseParenthesis, // )
    OpenBrace,        // {
    CloseBrace,       // }
    OpenBracket,      // [
    CloseBracket,     // ]
    LeftArrow,        // <-
    RightArrow,       // ->
    Collector,        // ~
    CollectorArrow,   // ~>
    Equals,           // =
    Unequals,         // !=
    GreaterThan,      // >
    LessThan,         // <
    GreaterEqual,     // >=
    LessEqual,        // <=
    Not,              // !
    Pipe,             // |
    PipePipe,         // ||
    And,              // &
    AndAnd,           // &&
    Plus,             // +
    Minus,            // -
    Slash,            // /
    Star,             // *

    // Literals
    String,           // "..."
    Number,           // 100

    // Keywords
    FuncKeyword,
    ExtKeyword,
    LocalKeyword,
    GlobalKeyword,
    ReturnKeyword,
    IfKeyword,
    ElseKeyword,
    WhileKeyword,
    BreakKeyword,
    ContinueKeyword,
    NullKeyword,

    // Identifiers
    Identifier,
} TokenType;

extern char* TokenTypeNames[Identifier+1];

// ============================================================================
// STRUCTS
// ============================================================================
typedef struct LEXER Lexer;
typedef struct TOKEN Token;
typedef struct TOKEN_LIST TokenList;

#define BUFFER_GROWTH_FACTOR 10

// Lexer object
struct LEXER {
    const char *SourceCode;
    int CurrentPointer;
    int SourceLength;

    Token *TokenBuffer;
    int BufferSize;
    int TokenCount;
};

// Token object
struct TOKEN {
    TokenType Type;
    char *Text;
    void *Value;
};

struct TOKEN_LIST {
    Token *Tokens;
    int Count;
};

// = ObjHelpers ===============================================================
void DeleteToken(Token tok);

// ============================================================================
// FUNCTIONS
// ============================================================================

void AppendToken(Lexer *lxr, Token tok);
char CurrentChar(Lexer *lxr);
char PeekChar(Lexer *lxr, int offset);
void PrintTokenList(TokenList lst);

TokenList Lex(char *source);
void LexKeyword(Lexer *lxr);
void LexNumber(Lexer *lxr);
void LexComment(Lexer *lxr);
void LexSymbol(Lexer *lxr);
void LexString(Lexer *lxr);
TokenType ClassifyKeyword(char *text);

#endif //CRCL_LEXER_H
