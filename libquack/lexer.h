#pragma once
#include <libquack/unit.h>
#include <libquack/integer.h>
#include <libquack/token.h>

typedef struct QkLexer QkLexer;
struct QkLexer {
    QkUnit* unit;
    char cur;
    i64 index;

    i64 line;
    i64 col;
    i64 lineIndex;
};

QkLexer qkLexerInit(QkUnit* unit);
QkToken qkLexerNext(QkLexer* lexer);
