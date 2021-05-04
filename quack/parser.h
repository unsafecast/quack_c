#pragma once
#include <quack/lexer.h>
#include <quack/ast.h>

typedef struct QkParser QkParser;
struct QkParser {
    QkLexer* lexer;
    QkUnit* unit;

    QkToken currentToken;
};

QkParser qkParserInit(QkLexer* lexer, QkUnit* unit);
QkStatement* qkParseStatement(QkParser* parser);
QkExpression* qkParseExpression(QkParser* parser);
