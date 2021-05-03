#pragma once
#include <quack/string.h>
#include <quack/dynArr.h>
#include <quack/error.h>

typedef struct QkUnit QkUnit;
struct QkUnit {
    QkString source;
    QkDynArr errLog;  // of QkError*
};

QkUnit qkUnitInit();
