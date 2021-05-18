#pragma once
#include <stdio.h>
#include <libquack/integer.h>
#include <libquack/string.h>

typedef enum QkConsoleFormat QkConsoleFormat;
enum QkConsoleFormat {
    QK_CONS_FMT_BOLD = (1 << 0),

    QK_CONS_FMT_RED  = (1 << 1),
    QK_CONS_FMT_BLUE = (1 << 2),
};

void qkFmtPrintf(FILE* stream, i64 fmt, const char* str, ...);
void qkFmtPrintString(FILE* stream, i64 fmt, const QkString* str);
