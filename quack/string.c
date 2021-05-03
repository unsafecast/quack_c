#include "./string.h"

void qkStringFromArray(QkString* dest, char* arr, i64 size) {
    dest->size = size;
    dest->data = arr;
}
