//
// Created by redcube on 15/01/23.
//
#include <stddef.h>
#include "binaryexpression.h"
#include "../../../Builtins/builtins.h"

#define NUM_OF_OPERATORS 8

BoundBinaryOperator Operators[NUM_OF_OPERATORS];

void InitOperators() {
    Operators [0] = (BoundBinaryOperator) { PlusOperator, Int32, Int32, Int32 };
    Operators [1] = (BoundBinaryOperator) { MinusOperator, Int32, Int32, Int32 };
    Operators [2] = (BoundBinaryOperator) { MultiplyOperator, Int32, Int32, Int32 };
    Operators [3] = (BoundBinaryOperator) { DivideOperator, Int32, Int32, Int32 };
    Operators [4] = (BoundBinaryOperator) { EqualOperator, Int32, Int32, Boolean };
    Operators [5] = (BoundBinaryOperator) { UnequalOperator, Int32, Int32, Boolean };
    Operators [6] = (BoundBinaryOperator) { EqualOperator, Int8Ptr, Int8Ptr, Boolean };
    Operators [7] = (BoundBinaryOperator) { UnequalOperator, Int8Ptr, Int8Ptr, Boolean };
}

BoundBinaryOperator *BindBinaryOperator(BinaryOperator op, TypeSymbol *left, TypeSymbol *right) {
    // search through all our known operators
    for (int i = 0; i < NUM_OF_OPERATORS; i++) {
        if (Operators[i].Operator == op && typcmp(left, Operators[i].Left) && typcmp(right, Operators[i].Right))
            return &Operators[i];
    }

    // found nothin
    return NULL;
}