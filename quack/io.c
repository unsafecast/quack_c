#include "./io.h"
#include "quack/string.h"
#include <quack/integer.h>
#include <stdlib.h>

QkString qkReadWholeFile(const char* path) {
    FILE* f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    i64 size = ftell(f);
    rewind(f);

    char* buffer = malloc(size);
    fread(buffer, sizeof(char), size, f);

    return qkStringFromArray(buffer, size);
}
