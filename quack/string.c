#include "./string.h"
#include <stdio.h>

QkString qkStringFromArray(char* arr, i64 size) {
    return (QkString) {
        .size = size,
        .data = arr,
    };
}

void qkPrintString(const QkString* str, FILE* stream) {
    fwrite(str->data, sizeof(char), str->size, stream);
}

QkString qkSliceString(const QkString* src, i64 start, i64 end) {
    return (QkString) {
        .data = src->data + start,
        .size = end - start,
    };
}

QkString qkSliceStringUntil(const QkString* src, i64 start, char end) {
    i64 i = start;
    for (; i < src->size; i += 1) {
        if (src->data[i] == end) break;
    }

    return qkSliceString(src, start, i);
}
