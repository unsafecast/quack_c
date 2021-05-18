#include "./fmt.h"
#include <stdarg.h>
#include <libquack/string.h>

#define COL_RED "\x1b[31m"
#define COL_BLUE "\x1b[34m"
#define BOLD "\x1b[1m"

#define RESET "\x1b[0m"

inline static void applyFormat(FILE* stream, i64 fmt) {
    if (fmt & QK_CONS_FMT_RED)  fputs(COL_RED, stream);
    if (fmt & QK_CONS_FMT_BLUE) fputs(COL_BLUE, stream);
    if (fmt & QK_CONS_FMT_BOLD) fputs(BOLD, stream);
}

inline static void resetFormat(FILE* stream) {
    fputs(RESET, stream);
}

void qkFmtPrintf(FILE* stream, i64 fmt, const char* str, ...) {
    va_list list;
    va_start(list, str);

    applyFormat(stream, fmt);
    vfprintf(stream, str, list);
    resetFormat(stream);

    va_end(list);
}

void qkFmtPrintString(FILE* stream, i64 fmt, const QkString* str) {
    applyFormat(stream, fmt);
    qkPrintString(str, stream);
    resetFormat(stream);
}
