#pragma once
#include <quack/unit.h>
#include <quack/integer.h>
#include <quack/token.h>

typedef struct QkLexer QkLexer;
struct QkLexer {
    QkUnit* unit;
    char cur;
    i64 index;
};

QkLexer qkLexerInit(QkUnit* unit);
QkToken qkLexerNext(QkLexer* lexer);
