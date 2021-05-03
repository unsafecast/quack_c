#pragma once
#include <quack/unit.h>
#include <quack/integer.h>

typedef struct QkLexer QkLexer;
struct QkLexer {
    QkUnit* unit;
    char cur;
    i64 index;
};

void qkLexerInit(QkLexer* lexer, QkUnit* unit);
