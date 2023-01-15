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