#include "./lexer.h"
#include "quack/dynArr.h"
#include "quack/string.h"
#include <stdbool.h>
#include <ctype.h>

inline static bool canIgnore(char c);
inline static void advance(QkLexer* l);

static QkToken getIdent(QkLexer* lexer);

QkLexer qkLexerInit(QkUnit* unit) {
    return (QkLexer) {
        .unit = unit,
        .index = 0,
        .cur = unit->source.data[0],
    };
}

QkToken qkLexerNext(QkLexer* lexer) {
    while (canIgnore(lexer->cur)) advance(lexer);

    if (lexer->index >= lexer->unit->source.size) return QK_TOKEN_EOF;
    else if (isalpha(lexer->cur)) return getIdent(lexer);

    qkDynArrPush(&lexer->unit->errLog, qkErrorUnexpChar(lexer->cur));
    return QK_TOKEN_NULL;
}


/*
    Private Functions
*/

static QkToken getIdent(QkLexer* lexer) {
    i64 start = lexer->index;
    while (isalnum(lexer->cur)) advance(lexer);
    return (QkToken) {
        .kind = QK_TOK_IDENT,
        .val_ident = qkSliceString(&lexer->unit->source, start, lexer->index),
    };
}

inline static void advance(QkLexer* l) {
    l->index += 1;
    l->cur = l->unit->source.data[l->index];
}

inline static bool canIgnore(char c) {
    return
        (c == '\n') ||
        (c == '\b') ||
        (c == '\r') ||
        (c == ' ')
    ;
}
