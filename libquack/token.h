#pragma once
#include <stdio.h>
#include <libquack/string.h>
#include <libquack/location.h>

typedef enum QkTokKind QkTokKind;
enum QkTokKind {
    QK_TOK_NULL,
    QK_TOK_EOF,

    QK_TOK_IDENT,
    QK_TOK_INT,

    QK_TOK_FUN,
    QK_TOK_CONST,
    QK_TOK_IF,
    QK_TOK_ELSE,
    QK_TOK_WHILE,
    QK_TOK_STRUCT,

    QK_TOK_COL,
    QK_TOK_COL_EQ,
    QK_TOK_SEMI,
    QK_TOK_STAR,
    QK_TOK_EQ,
    QK_TOK_CURLY_OPEN,
    QK_TOK_CURLY_CLOSE,
    QK_TOK_PAREN_OPEN,
    QK_TOK_PAREN_CLOSE,
    QK_TOK_THIN_ARROW,
    QK_TOK_COMMA,
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
