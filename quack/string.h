#pragma once
#include <quack/integer.h>
#include <stdio.h>

typedef struct QkString QkString;
struct QkString {
    i64   size;
    char* data;
};

QkString qkStringFromArray(char* arr, i64 size);
void qkPrintString(const QkString* str, FILE* stream);

QkString qkSliceString(const QkString* src, i64 start, i64 end);
QkString qkSliceStringUntil(const QkString* src, i64 start, char end);
