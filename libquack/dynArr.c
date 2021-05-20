#include "./dynArr.h"
#include <stdlib.h>

QkDynArr qkDynArrInit(i64 initialCap) {
    if (initialCap < 1) initialCap = 1;

    QkDynArr arr;
    arr.data = malloc(initialCap * sizeof(void*));
    arr.cap = initialCap;
    arr.size = 0;
    
    return arr;
}

void qkDynArrPush(QkDynArr* arr, void* item) {
    if (arr->cap <= arr->size) {
        arr->cap += 1;
        arr->data = realloc(arr->data, arr->cap * sizeof(void*));
    }

    arr->data[arr->size] = item;
    arr->size += 1;
}
