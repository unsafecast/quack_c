#include "./dynArr.h"
#include <stdlib.h>

QkDynArr qkDynArrInit(i64 initialCap) {
    if (initialCap < 1) initialCap = 1;

    return (QkDynArr) {
        .data = malloc(initialCap * sizeof(void*)),
        .cap = initialCap,
        .size = 0,
    };
}

void qkDynArrPush(QkDynArr* arr, void* item) {
    if (arr->cap <= arr->size) {
        arr->cap += 1;
        arr->data = realloc(arr->data, arr->cap * sizeof(void*));
    }

    arr->data[arr->size] = item;
    arr->size += 1;
}
