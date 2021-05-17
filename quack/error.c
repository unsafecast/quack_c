#include "./error.h"
#include "quack/location.h"
#include "quack/string.h"
#include "quack/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <quack/fmt.h>
#include <math.h>

static void printOffset(i64 column, char c, FILE* stream);
static i64 numberOfDigits(i64 number);
static char* showTokenKind(QkTokKind kind);
static void printToken(FILE* stream, i64 fmt, const QkToken* tok);

QkError* qkErrorUnexpChar(QkLocation loc, char c) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_UNEXP_CHAR;
    e->valUnexpChar = c;
    e->location = loc;
    
    return e;
}

QkError* qkErrorExpToken(QkLocation loc, QkTokKind expected, const QkToken* got) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_EXP_TOKEN;
    e->valExpToken.expected = expected;
    e->valExpToken.got = *got;
    e->location = loc;

    return e;
}

QkError* qkErrorUnexpToken(QkLocation loc, const QkToken* token) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_UNEXP_TOKEN;
    e->location = loc;
    e->unexpectedToken = *token;

    return e;
}

// TODO: Make this a good error printer (with source code and stuff)
void qkPrintError(const QkError* err, const QkString* source, FILE* stream) {
    qkFmtPrintf(stream, QK_CONS_FMT_BOLD | QK_CONS_FMT_RED, "Error(%lld:%lld): ",
        err->location.line, err->location.col);
    
    QkString line = qkSliceStringUntil(source, err->location.lineIndex, '\n');

    const i64 MSG_FMT = QK_CONS_FMT_BOLD;
    switch (err->kind) {
        case QK_ERR_UNEXP_CHAR:
            qkFmtPrintf(stream, MSG_FMT, "Unexpected character '%c'!\n", err->valUnexpChar);
            break;

        case QK_ERR_EXP_TOKEN:
            qkFmtPrintf(stream, MSG_FMT, "Unexpected token ");
            printToken(stream, MSG_FMT, &err->valExpToken.got);
            qkFmtPrintf(stream, MSG_FMT, ", expected %s instead\n", showTokenKind(err->valExpToken.expected));
            break;

        case QK_ERR_UNEXP_TOKEN:
            qkFmtPrintf(stream, MSG_FMT, "Unexpected token ");
            printToken(stream, MSG_FMT, &err->unexpectedToken);
            qkFmtPrintf(stream, MSG_FMT, "\n");
            break;
    }

    const i64 POS_FMT = QK_CONS_FMT_BOLD | QK_CONS_FMT_BLUE;
    qkFmtPrintf(stream, POS_FMT, "%lld │  ", err->location.line);
    qkPrintString(&line, stream);
    fputc('\n', stream);


    // FIXME: Cleanup! Maybe put these into a function, I'm not sure of the correct
    //  way to abstract it though
    printOffset(numberOfDigits(err->location.line), ' ', stream);
    qkFmtPrintf(stream, POS_FMT, " │  ");
    printOffset(err->location.col - 1, ' ', stream);
    qkFmtPrintf(stream, POS_FMT, "│\n");

    printOffset(numberOfDigits(err->location.line), ' ', stream);
    qkFmtPrintf(stream, POS_FMT, " │  ");
    printOffset(err->location.col - 1, ' ', stream);
    qkFmtPrintf(stream, POS_FMT, "└─ here!\n");


    // Example error output:
    //
    //  Error(1:4): Unexpected token ':', expected 'e' instead
    //  1 │  som: thing
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

static char* showTokenKind(QkTokKind kind) {
    switch (kind) {
        case QK_TOK_COL_EQ: return "':='";
        case QK_TOK_COL:    return "':'";
        case QK_TOK_SEMI:   return "';'";
        case QK_TOK_NULL:   return "<null>";
        case QK_TOK_INT:    return "an integer";
        case QK_TOK_IDENT:  return "an identifier";
        case QK_TOK_EOF:    return "end of file";
        case QK_TOK_EQ:     return "'='";
        case QK_TOK_STAR:   return "'*'";
        case QK_TOK_FUN:    return "'fun'";

        case QK_TOK_CURLY_OPEN:  return "'{'";
        case QK_TOK_CURLY_CLOSE: return "'}'";
    }
}

static void printToken(FILE* stream, i64 fmt, const QkToken* tok) {
    switch (tok->kind) {
        case QK_TOK_COL_EQ:
            qkFmtPrintf(stream, fmt, "':='");
            break;
        case QK_TOK_COL:
            qkFmtPrintf(stream, fmt, "':'");
            break;
        case QK_TOK_SEMI:
            qkFmtPrintf(stream, fmt, "';'");
            break;
        case QK_TOK_NULL:
            qkFmtPrintf(stream, fmt, "<null>");
            break;
        case QK_TOK_INT:
            qkFmtPrintf(stream, fmt, "'%lld'", tok->valInt);
            break;
        case QK_TOK_IDENT:
            qkFmtPrintf(stream, fmt, "'");
            qkFmtPrintString(stream, fmt, &tok->valIdent);
            qkFmtPrintf(stream, fmt, "'");
            break;
        case QK_TOK_EOF:
            qkFmtPrintf(stream, fmt, "end of file");
            break;
        case QK_TOK_EQ:
            qkFmtPrintf(stream, fmt, "'='");
            break;
        case QK_TOK_STAR:
            qkFmtPrintf(stream, fmt, "'*'");
            break;
        case QK_TOK_CURLY_OPEN:
            qkFmtPrintf(stream, fmt, "'{'");
            break;
        case QK_TOK_CURLY_CLOSE:
            qkFmtPrintf(stream, fmt, "'}'");
            break;
        case QK_TOK_FUN:
            qkFmtPrintf(stream, fmt, "'fun'");
            break;
    }
}
