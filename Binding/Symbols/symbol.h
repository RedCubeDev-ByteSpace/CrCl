//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_SYMBOL_H
#define CRCL_SYMBOL_H

extern int IdCounter;
int GetUniqueId();

typedef enum SYMBOL_TYPE {
    TypeSymbolType,
    FunctionSymbolType,
    LocalVariableSymbolType,
    GlobalVariableSymbolType,
    ParameterSymbolType,
} SymbolType;


typedef struct SYMBOL {
    SymbolType Type;
    char *Name;
} Symbol;

typedef struct SYMBOL_LIST {
    Symbol **Symbols;
    int Count;
} SymbolList;

#endif //CRCL_SYMBOL_H
