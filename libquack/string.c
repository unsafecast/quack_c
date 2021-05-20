#include "./string.h"
#include <stdio.h>
#include <string.h>

QkString qkStringFromArray(char* arr, i64 size) {
    QkString string;
    string.size = size;
    string.data = arr;
    
    return string;
}

void qkPrintString(const QkString* str, FILE* stream) {
    fwrite(str->data, sizeof(char), str->size, stream);
}

QkString qkSliceString(const QkString* src, i64 start, i64 end) {
    QkString string;
    string.data = src->data + start;
    string.size = end - start;
    
    return string;
}

QkString qkSliceStringUntil(const QkString* src, i64 start, char end) {
    i64 i = start;
    for (; i < src->size; i += 1) {
        if (src->data[i] == end) break;
    }

    return qkSliceString(src, start, i);
}

bool qkStringEq(const QkString* s1, const QkString* s2) {
    if (s1->size != s2->size) return false;

    for (i64 i = 0; i < s1->size; i += 1) {
        if (s1->data[i] != s2->data[i]) return false;
    }

    return true;
}

bool qkStringArrEq(const QkString* s1, const char* s2) {
    QkString str = qkStringFromArray((char*)s2, strlen(s2));
    return qkStringEq(s1, &str);
}
