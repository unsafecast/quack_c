#pragma once
#include <quack/integer.h>

typedef struct QkDynArr QkDynArr;
struct QkDynArr {
    void** data;
    i64 cap;
    i64 size;
};

QkDynArr qkDynArrInit(i64 initialCap);
void qkDynArrPush(QkDynArr* arr, void* item);

#define QK_FOR(arr) for (i64 it = 0; it < (arr)->size; it += 1)
