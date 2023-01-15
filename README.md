# CrCl Source to Source Compiler
```
CrCl source file ---> [CrCl StS Compiler] ---> C source file
```

CrCl is a low level language very similar to ReCT in style. While ReCT compiles down to LLVM, CrCl compiles to the poor 
man's intermediate language: C.  
I started this project because I wanted to run ReCT on a calculator, but it was just too heavy, so I decided to make this
second language for that purpose.  
That is also why I decided to compile to C instead of LLVM as LLVM does not currently have a backend to compile to the 
calculator's architecture (SH4) so compiling `CrCl -> C -> SH4 Machine Code` is a lot nicer than `ReCT -> LLVM -> C -> SH4 Machine Code`.

This language is basically C but the way I'd like C to be (which explains all the ReCT-y stuff).

### Example
```c
func main() int32 {
    println("Hello World!");
    return 0;
}

func println(msg ptr[int8]) {
    printf("%s\n", msg);
}

ext printf(fmt ptr[int8], ~);
```

The piece of code above generates the following C-code:
```c
//crcl generated code
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

int32_t main();
void println(int8_t* msg);
void printf(int8_t* fmt, ...);

int32_t main() {
    (println(("Hello World!")));
    return (0);
}

void println(int8_t* msg) {
    (printf(("%s\n"), (msg)));
}
```

Yes I wrote an entire compiler to fuck around with a calculator.