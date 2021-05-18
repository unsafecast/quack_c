#pragma once
#include <libquack/lexer.h>
#include <libquack/ast.h>

typedef struct QkParser QkParser;
struct QkParser {
    QkLexer* lexer;
    QkUnit* unit;

    QkToken currentToken;
    QkToken nextToken;
};

QkParser qkParserInit(QkLexer* lexer, QkUnit* unit);
QkStatement* qkParseStatement(QkParser* parser);
QkExpression* qkParseExpression(QkParser* parser);
