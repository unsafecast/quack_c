#pragma once
#include <quack/string.h>

typedef enum QkTokKind QkTokKind;
enum QkTokKind {
    QK_TOK_IDENT,
};

typedef struct QkToken QkToken;
struct QkToken {
    QkTokKind kind;

    union {
        QkString val_ident;
    };
};
