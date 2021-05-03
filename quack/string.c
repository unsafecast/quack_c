#include "./string.h"
#include <stdio.h>

QkString qkStringFromArray(char* arr, i64 size) {
    return (QkString) {
        .size = size,
        .data = arr,
    };
}

void qkPrintString(QkString* str, FILE* stream) {
    fwrite(str->data, sizeof(char), str->size, stream);
}

QkString qkSliceString(QkString* src, i64 start, i64 end) {
    return (QkString) {
        .data = src->data + start,
        .size = end - start,
    };
}

