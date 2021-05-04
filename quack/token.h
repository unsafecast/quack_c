#pragma once
#include <quack/string.h>
#include <quack/location.h>
#include <stdio.h>

typedef enum QkTokKind QkTokKind;
enum QkTokKind {
    QK_TOK_NULL,
    QK_TOK_EOF,

    QK_TOK_IDENT,
    QK_TOK_INT,

    QK_TOK_COL,
    QK_TOK_COL_EQ,
    QK_TOK_SEMI,
};

typedef struct QkToken QkToken;
struct QkToken {
    QkTokKind kind;

    QkLocation loc;

    union {
        QkString valIdent;
        i64 valInt;
    };
};

#define QK_TOKEN(_kind, _loc) (QkToken) { .kind = _kind, .loc = _loc, }

void qkPrintToken(const QkToken* tok, FILE* stream);
