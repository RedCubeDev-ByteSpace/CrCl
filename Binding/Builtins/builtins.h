//
// Created by redcube on 15/01/23.
//

#ifndef CRCL_BUILTINS_H
#define CRCL_BUILTINS_H

#include "../Symbols/typesymbol.h"

extern TypeSymbol *Boolean;

extern TypeSymbol *Int8;
extern TypeSymbol *Int16;
extern TypeSymbol *Int32;
extern TypeSymbol *Int64;

extern TypeSymbol *Int8Ptr;

extern TypeSymbol *Void;

extern TypeSymbol *VArgs;

void InitBuiltins();

#endif //CRCL_BUILTINS_H
