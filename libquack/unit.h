#pragma once
#include <libquack/string.h>
#include <libquack/dynArr.h>
#include <libquack/error.h>

typedef struct QkUnit QkUnit;
struct QkUnit {
    QkString source;
    QkDynArr errLog;  // of QkError*
};

QkUnit qkUnitInit();
