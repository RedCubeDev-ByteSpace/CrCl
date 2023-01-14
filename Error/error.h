//
// Created by redcube on 14/01/23.
//

#ifndef CRCL_ERROR_H
#define CRCL_ERROR_H

// Colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// Helper thingies
void cprintf(char *color, char *msg, ...);

// Error thingies
void Die(char *ErrMessage, ...);

#endif //CRCL_ERROR_H
