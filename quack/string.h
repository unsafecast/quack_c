#pragma once
#include <quack/integer.h>

typedef struct QkString QkString;
struct QkString {
    i64   size;
    char* data;
};

void qkStringFromArray(QkString* dest, char* arr, i64 size);
