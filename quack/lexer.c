#include "./lexer.h"
#include "quack/location.h"
#include "quack/token.h"
#include <quack/dynArr.h>
#include <quack/string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define PEEK(l) (l)->unit->source.data[(l)->index + 1]

inline static bool canIgnore(char c);
inline static char advance(QkLexer* l);
inline static QkLocation getLocation(QkLexer* lexer);

static QkToken getIdent(QkLexer* lexer);
static QkToken getNumber(QkLexer* lexer);

QkLexer qkLexerInit(QkUnit* unit) {
    return (QkLexer) {
        .unit = unit,
        .index = 0,
        .cur = unit->source.data[0],
        .line = 1,
        .col = 1,
        .lineIndex = 0,
    };
}

QkToken qkLexerNext(QkLexer* lexer) {
    while (canIgnore(lexer->cur)) advance(lexer);

    if (lexer->index >= lexer->unit->source.size) return QK_TOKEN(QK_TOK_EOF, getLocation(lexer));
    else if (isalpha(lexer->cur)) return getIdent(lexer);
    else if (isdigit(lexer->cur)) return getNumber(lexer);
    else {
        QkLocation loc = getLocation(lexer);
        switch (lexer->cur) {
            case ':':
                switch (advance(lexer)) {
                    case '=': advance(lexer); return QK_TOKEN(QK_TOK_COL_EQ, loc);
                    default: return QK_TOKEN(QK_TOK_COL, loc);
                }
            
            case ';':
                advance(lexer);
                return QK_TOKEN(QK_TOK_SEMI, loc);

            case '*':
                advance(lexer);
                return QK_TOKEN(QK_TOK_STAR, loc);

            case '=':
                advance(lexer);
                return QK_TOKEN(QK_TOK_EQ, loc);
        }
    }

    qkDynArrPush(&lexer->unit->errLog, qkErrorUnexpChar(getLocation(lexer), lexer->cur));
    return QK_TOKEN(QK_TOK_NULL, getLocation(lexer));
}


/*
    Private Functions
*/

static QkToken getIdent(QkLexer* lexer) {
    QkLocation loc = getLocation(lexer);

    i64 start = lexer->index;
    while (isalnum(lexer->cur)) advance(lexer);
    return (QkToken) {
        .kind = QK_TOK_IDENT,
        .valIdent = qkSliceString(&lexer->unit->source, start, lexer->index),
        .loc = loc,
    };
}

static QkToken getNumber(QkLexer* lexer) {
    QkLocation loc = getLocation(lexer);

    i64 start = lexer->index;
    while (isdigit(lexer->cur)) advance(lexer);  // TODO: Add support for floats
    return (QkToken) {
        .kind = QK_TOK_INT,
        .valInt = strtol(qkSliceString(&lexer->unit->source, start, lexer->index).data, NULL, 10),
        .loc = loc,
    };
}

inline static QkLocation getLocation(QkLexer* lexer) {
    return (QkLocation) {
        .line = lexer->line,
        .col = lexer->col,
        .index = lexer->index,
        .lineIndex = lexer->lineIndex,
    };
}

inline static char advance(QkLexer* l) {
    if (l->cur == '\n') {
        l->line += 1;

        // This is to omit the newline, the line itself doesn't really contain it
        l->lineIndex = l->index + 1;

        // We set it to 0, not 1, because the "first" one is actually the newline,
        //  which we ignored on the line above
        l->col = 0;
    }

    l->index += 1;
    l->cur = l->unit->source.data[l->index];
    l->col += 1;

    return l->cur;
}

inline static bool canIgnore(char c) {
    return
        (c == '\n') ||
        (c == '\b') ||
        (c == '\r') ||
        (c == ' ')
    ;
}
