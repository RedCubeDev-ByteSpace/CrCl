//
// Created by redcube on 15/01/23.
//
#include <gc/gc.h>
#include "builtins.h"

TypeSymbol *Boolean;
TypeSymbol *Int8;
TypeSymbol *Int16;
TypeSymbol *Int32;
TypeSymbol *Int64;

TypeSymbol *Int8Ptr;

TypeSymbol *Void;

TypeSymbol *VArgs;

void InitBuiltins() {
    // create some default type symbols

    // BOOL
    Boolean = GC_MALLOC(sizeof(TypeSymbol));
    Boolean->base.Type = TypeSymbolType;
    Boolean->base.Name = "bool";
    Boolean->Subtypes = (TypeSymbolList) {0, 0};

    // ALL THEM INTS
    Int8 = GC_MALLOC(sizeof(TypeSymbol));
    Int8->base.Type = TypeSymbolType;
    Int8->base.Name = "int8";
    Int8->Subtypes = (TypeSymbolList) {0, 0};

    Int16 = GC_MALLOC(sizeof(TypeSymbol));
    Int16->base.Type = TypeSymbolType;
    Int16->base.Name = "int16";
    Int16->Subtypes = (TypeSymbolList) {0, 0};

    Int32 = GC_MALLOC(sizeof(TypeSymbol));
    Int32->base.Type = TypeSymbolType;
    Int32->base.Name = "int32";
    Int32->Subtypes = (TypeSymbolList) {0, 0};

    Int64 = GC_MALLOC(sizeof(TypeSymbol));
    Int64->base.Type = TypeSymbolType;
    Int64->base.Name = "int64";
    Int64->Subtypes = (TypeSymbolList) {0, 0};

    Int8Ptr = GC_MALLOC(sizeof(TypeSymbol));
    Int8Ptr->base.Type = TypeSymbolType;
    Int8Ptr->base.Name = "ptr";
    Int8Ptr->Subtypes = (TypeSymbolList) {0, 1};
    Int8Ptr->Subtypes.Symbols = GC_MALLOC(sizeof(TypeSymbol*));
    Int8Ptr->Subtypes.Symbols[0] = Int8;

    // cum
    Void = GC_MALLOC(sizeof(TypeSymbol));
    Void->base.Type = TypeSymbolType;
    Void->base.Name = "void";
    Void->Subtypes = (TypeSymbolList) {0, 0};

    // variadic args
    VArgs = GC_MALLOC(sizeof(TypeSymbol));
    VArgs->base.Type = TypeSymbolType;
    VArgs->base.Name = "vargs";
    VArgs->Subtypes = (TypeSymbolList) {0, 0};
}