#include <stdio.h>
#include <stdlib.h>

#include "Lexing/lexer.h"
#include "Error/error.h"

// Writing a compiler in C because i hate myself
// also i like challenges i guess
// wanted to write it in go but didnt find any motivation so we're going for suffering instead

// its probably not going to compile to llvm because thats not really necessary
// i just want to fuck around with my casio calculator and write some garbage software
// so the compile target for this compiler will be C

// CrCl ---> [CrCL Source to Source Compiler] ---> C

int main(int argc, char** argv) {
    if (argc != 2) return -1; // we need one source file

    // open the source file
    FILE *sourceFile = fopen(argv[1], "r");

    // find length of source file
    fseek(sourceFile, 0, SEEK_END);
    long fileLength = ftell(sourceFile);

    // rewind file back to the start
    rewind(sourceFile);

    // read out all of the file's contents
    char *source = malloc(fileLength + 1);
    fread(source, fileLength, 1, sourceFile);

    // null terminator
    source[fileLength] = 0;

    // we're done with the file
    fclose(sourceFile);

    printf("CrCL Gaming Cross Compiler for Gaming (gaming)\n");
    printf("----------------------------------------------\n\n");
    printf("Input file:\n");
    printf("-----------\n");
    printf("%s\n\n", source);

    TokenList Tokens = Lex(source);

    printf("Tokens:\n");
    printf("-------\n");
    PrintTokenList(Tokens);

    return 0;
}
