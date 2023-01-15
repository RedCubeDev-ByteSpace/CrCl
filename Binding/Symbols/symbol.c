//
// Created by redcube on 15/01/23.
//
#include "symbol.h"

int IdCounter = 0;

int GetUniqueId() {
    return IdCounter++;
}