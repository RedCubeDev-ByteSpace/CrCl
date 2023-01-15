#include <stdio.h>
#include <stdlib.h>
#include <gc.h>

#include "Lexing/lexer.h"
#include "Error/error.h"
#include "Parsing/parser.h"

// Writing a compiler in C because i hate myself
// also i like challenges i guess
// wanted to write it in go but didnt find any motivation so we're going for suffering instead

// its probably not going to compile to llvm because thats not really necessary
// i just want to fuck around with my casio calculator and write some garbage software
// so the compile target for this compiler will be C

// CrCl ---> [CrCL Source to Source Compiler] ---> C

int main(int argc, char** argv) {
    if (argc != 2) return -1; // we need one source file

    // garbage collector because i am fatherless
    GC_INIT();

    // open the source file
    FILE *sourceFile = fopen(argv[1], "r");

    // find length of source file
    fseek(sourceFile, 0, SEEK_END);
    long fileLength = ftell(sourceFile);

    // rewind file back to the start
    rewind(sourceFile);

    // read out all of the file's contents
    char *source = GC_MALLOC(fileLength + 1);
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

    TokenList tokens = Lex(source);

    printf("Tokens:\n");
    printf("-------\n");
    PrintTokenList(tokens);

    // :b:arsing
    printf("Members:\n");
    printf("--------\n");

    NodeList nodes = Parse(tokens);

    for (int i = 0; i < nodes.Count; i++) {
        FunctionMemberNode *fnc = nodes.NodeBuffer[i];
        printf("Function %s\n Returns: %s\n Number of parameters: %d\n\n", fnc->Identifier.Text, fnc->ReturnType == NULL ? "void" : fnc->ReturnType->Identifier.Text, fnc->Parameters->Count);
    }

    // The exit status of a process in computer programming is a small number passed from a child process (or callee) to a parent process (or caller)
    // when it has finished executing a specific procedure or delegated task. In DOS, this may be referred to as an error level.

    // When computer programs are executed, the operating system creates an abstract entity called a process in which the bookkeeping for that
    // program is maintained. In multitasking operating systems such as Unix or Linux, new processes can be created by active processes. The process
    // that spawns another is called a parent process, while those created are child processes. Child processes run concurrently with the parent process.
    // The technique of spawning child processes is used to delegate some work to a child process when there is no reason to stop the execution of the parent.
    // When the child finishes executing, it exits by calling the exit system call. This system call facilitates passing the exit status code back
    // to the parent, which can retrieve this value using the wait system call.

    // The C programming language allows programs exiting or returning from the main function to signal success or failure by returning an integer, or
    // returning the macros EXIT_SUCCESS and EXIT_FAILURE. On Unix-like systems these are equal to 0 and 1 respectively.[3] A C program may also use the

    // The return value from main is passed to the exit function, which for values zero, EXIT_SUCCESS or EXIT_FAILURE may translate it to “an implementation
    // defined form” of successful termination or unsuccessful termination.

    // Apart from zero and the macros EXIT_SUCCESS and EXIT_FAILURE, the C standard does not define the meaning of return codes. Rules for the use of return
    // codes vary on different platforms (see the platform-specific sections).

    // we return 0 because 0 is the exit code for success, meaning there was no errors :^)
    return 0;
}
