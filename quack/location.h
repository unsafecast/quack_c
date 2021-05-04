#pragma once
#include <quack/integer.h>
#include <stdio.h>

typedef struct QkLocation QkLocation;
struct QkLocation {
    i64 line;
    i64 col;
    i64 index;
    i64 lineIndex;
};

void qkPrintLocation(const QkLocation* loc, FILE* stream);
