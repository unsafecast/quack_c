#include "./error.h"
#include "quack/location.h"
#include "quack/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <quack/fmt.h>

static void printOffset(i64 column, char c, FILE* stream);

QkError* qkErrorUnexpChar(QkLocation loc, char c) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_UNEXP_CHAR;
    e->val_unexp_char = c;
    e->location = loc;
    
    return e;
}

// TODO: Make this a good error printer (with source code and stuff)
void qkPrintError(const QkError* err, const QkString* source, FILE* stream) {
    qkPrintf(stream, QK_CONS_FMT_BOLD | QK_CONS_FMT_RED, "Error(%lld:%lld): ",
        err->location.line, err->location.col);
    
    QkString line = qkSliceStringUntil(source, err->location.lineIndex, '\n');

    switch (err->kind) {
        case QK_ERR_UNEXP_CHAR:
            qkPrintf(stream, QK_CONS_FMT_BOLD, "Unexpected character '%c'!\n", err->val_unexp_char);
            break;
    }

    qkPrintf(stream, QK_CONS_FMT_BOLD | QK_CONS_FMT_BLUE, "%lld |\t", err->location.line);
    qkPrintString(&line, stream);
    fputc('\n', stream);

    // FIXME: I still have no idea why 7 (i'd think it's 4) but yeah, it works
    printOffset(7 + err->location.col, ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, "^\n");

    printOffset(7 + err->location.col, ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, "|_ here!\n");


    //
    //  1 |    something
    //            ^
    //            |_ here!
    //
}

static void printOffset(i64 column, char c, FILE* stream) {
    while (column > 0) {
        column--;
        fputc(c, stream);
    }
}
