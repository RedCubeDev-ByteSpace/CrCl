# CrCl Source to Source Compiler
```
CrCl source file ---> [CrCl StS Compiler] ---> C source file
```

CrCl is a low level language very similar to ReCT in style. While ReCT compiles down to LLVM, CrCl compiles to the poor 
man's intermediate language C.  
I started this project because I wanted to run ReCT on a calculator, but it was just too heavy, so I decided to make this
second language for that purpose.  
That is also why I decided to compile to C instead of LLVM as LLVM does not currently have a backend to compile to the 
calculator's architecture (SH4) so compiling `CrCl -> C -> SH4 Machine Code` is a lot nicer than `ReCT -> LLVM -> C -> SH4 Machine Code`.

This language is basically C but the way I'd like C to be (which explains all the ReCT-y stuff).

### Example
```c
func main() int32 {
    PrintHelloWorld();
    return 0;
}

func PrintHelloWorld() {
    puts("Hello world!");
}

// CrCl is very early in development and does not yet have a way of referencing external functions
// so best i can do is declare it with an empty body and remove the definition from the C output later
func puts(msg ptr[int8]) {}
```

The piece of code above generates the following C-code:
```c
//crcl generated code
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

int32_t main();
void PrintHelloWorld();
void puts(int8_t* msg);

int32_t main()
{
    (PrintHelloWorld());
    return (0);
}

void PrintHelloWorld()
{
    (puts(("Hello world!")));
}

// manually commented out be me because of the missing feature i mentioned above
// void puts(int8_t* msg)
// {
// }
```

Yes I wrote an entire compiler to fuck around with a calculator.