#pragma once
#include <quack/string.h>
#include <stdio.h>

typedef enum QkTokKind QkTokKind;
enum QkTokKind {
    QK_TOK_NULL,
    QK_TOK_EOF,

    QK_TOK_IDENT,
};

typedef struct QkToken QkToken;
struct QkToken {
    QkTokKind kind;

    union {
        QkString val_ident;
    };
};

#define QK_TOKEN_NULL (QkToken) { .kind = QK_TOK_NULL, }
#define QK_TOKEN_EOF  (QkToken) { .kind = QK_TOK_EOF, }

void qkPrintToken(QkToken* tok, FILE* stream);
