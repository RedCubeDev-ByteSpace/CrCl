//
// Created by redcube on 15/01/23.
//

#include "../../../../Lexing/lexer.h"

int BinaryOperatorPrecedence(Token tok) {
    switch (tok.Type) {
        case Star:
        case Slash:
            return 5;

        case Plus:
        case Minus:
            return 4;

        case Equals:
        case Unequals:
        case LessThan:
        case LessEqual:
        case GreaterThan:
        case GreaterEqual:
            return 3;

        case And:
        case AndAnd:
            return 2;

        case Pipe:
        case PipePipe:
            return 1;

        default:
            return 0;
    }
}