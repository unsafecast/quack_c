#include "./parser.h"
#include "quack/dynArr.h"
#include "quack/error.h"
#include <quack/lexer.h>
#include <quack/token.h>
#include <stdlib.h>

static QkToken* advance(QkParser* parser);
static QkToken* expect(QkParser* parser, QkTokKind kind);

static QkStatement* parseAssign(QkParser* parser, QkExpression* name);

QkParser qkParserInit(QkLexer* lexer, QkUnit* unit) {
    return (QkParser) {
        .lexer = lexer,
        .unit = unit,
    };
}

QkStatement* qkParseStatement(QkParser* parser) {
    // Check for keywords like "fn" here

    QkExpression* expr = qkParseExpression(parser);
    if (expr == NULL) return NULL;

    switch (advance(parser)->kind) {
        case QK_TOK_COL_EQ:
            return parseAssign(parser, expr);
        
        default: {
            QkStatement* stmt = malloc(sizeof(QkStatement));
            *stmt = (QkStatement) {
                .kind = QK_STMT_KIND_EXPR,
                .valExpr = expr,
            };
            return stmt;
        }
    }
}

QkExpression* qkParseExpression(QkParser* parser) {
    switch (advance(parser)->kind) {
        case QK_TOK_IDENT: {
            QkExpression* e = malloc(sizeof(QkExpression));
            *e = (QkExpression) {
                .kind = QK_EXPR_KIND_IDENT,
                .loc = parser->currentToken.loc,
                .valString = parser->currentToken.valIdent,
            };
            return e;
        }

        case QK_TOK_INT: {
            QkExpression* e = malloc(sizeof(QkExpression));
            *e = (QkExpression) {
                .kind = QK_EXPR_KIND_INT_LIT,
                .loc = parser->currentToken.loc,
                .valIntLit = parser->currentToken.valInt,
            };
            return e;
        }

        case QK_TOK_EOF: {
            QkExpression* e = malloc(sizeof(QkExpression));
            *e = (QkExpression) {
                .kind = QK_EXPR_KIND_EOF,
                .loc = parser->currentToken.loc,
            };
            return e;
        }

        default: break;
    }

    qkDynArrPush(&parser->unit->errLog, qkErrorUnexpToken(parser->currentToken.loc));
    return NULL;
}

static QkStatement* parseAssign(QkParser* parser, QkExpression* name) {
    QkExpression* value = qkParseExpression(parser);
    if (value == NULL) return NULL;

    expect(parser, QK_TOK_SEMI);

    QkStatement* stmt = malloc(sizeof(QkStatement));
    *stmt = (QkStatement) {
        .kind = QK_STMT_KIND_ASSIGN,
        .loc = name->loc,
        .valAssign.name = name,
        .valAssign.value = value,
    };
    return stmt;
}

static QkToken* advance(QkParser* parser) {
    parser->currentToken = qkLexerNext(parser->lexer);
    return &parser->currentToken;
}

static QkToken* expect(QkParser* parser, QkTokKind kind) {
    QkToken* token = advance(parser);
    if (token->kind != kind) {
        qkDynArrPush(&parser->unit->errLog, qkErrorExprToken(token->loc, kind));
    }

    return token;
}
