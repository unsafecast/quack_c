#include "./error.h"
#include "quack/location.h"
#include "quack/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <quack/fmt.h>
#include <math.h>

static void printOffset(i64 column, char c, FILE* stream);
static i64 numberOfDigits(i64 number);
static char* tokenToReadable(QkTokKind kind);

QkError* qkErrorUnexpChar(QkLocation loc, char c) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_UNEXP_CHAR;
    e->valUnexpChar = c;
    e->location = loc;
    
    return e;
}

QkError* qkErrorExprToken(QkLocation loc, QkTokKind kind) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_EXP_TOKEN;
    e->valExprToken = kind;
    e->location = loc;

    return e;
}

QkError* qkErrorUnexpToken(QkLocation loc) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_UNEXP_TOKEN;
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
            qkPrintf(stream, QK_CONS_FMT_BOLD, "Unexpected character '%c'!\n", err->valUnexpChar);
            break;

        case QK_ERR_EXP_TOKEN:
            qkPrintf(stream, QK_CONS_FMT_BOLD, "Unexpected token, expected %s!\n", tokenToReadable(err->valExprToken));
            break;

        case QK_ERR_UNEXP_TOKEN:
            qkPrintf(stream, QK_CONS_FMT_BOLD, "Unexpected token!\n");
            break;
    }

    qkPrintf(stream, QK_CONS_FMT_BOLD | QK_CONS_FMT_BLUE, "%lld │  ", err->location.line);
    qkPrintString(&line, stream);
    fputc('\n', stream);


    // FIXME: Cleanup! Maybe put these into a function, I'm not sure of the correct
    //  way to abstract it though
    printOffset(numberOfDigits(err->location.line), ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, " │  ");
    printOffset(err->location.col - 1, ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, "│\n");

    printOffset(numberOfDigits(err->location.line), ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, " │  ");
    printOffset(err->location.col - 1, ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, "│\n");

    printOffset(numberOfDigits(err->location.line), ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, " │  ");
    printOffset(err->location.col - 1, ' ', stream);
    qkPrintf(stream, QK_CONS_FMT_BLUE | QK_CONS_FMT_BOLD, "└─ here!\n");


    //
    //  1 │  something
    //    │     │
    //    │     │
    //    │     └─ here!
    //
}

static void printOffset(i64 column, char c, FILE* stream) {
    while (column > 0) {
        column--;
        fputc(c, stream);
    }
}

static i64 numberOfDigits(i64 number) {
    return (i64)(log10(number) + 1);
}

static char* tokenToReadable(QkTokKind kind) {
    switch (kind) {
        case QK_TOK_COL_EQ: return "':='";
        case QK_TOK_COL:    return "':'";
        case QK_TOK_SEMI:   return "';'";
        case QK_TOK_NULL:   return "<null>";
        case QK_TOK_INT:    return "an integer";
        case QK_TOK_IDENT:  return "an identifier";
        case QK_TOK_EOF:    return "end of file";
    }
}
