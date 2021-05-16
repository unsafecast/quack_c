#include "./parser.h"
#include <quack/ast.h>
#include <quack/dynArr.h>
#include <quack/error.h>
#include <quack/string.h>
#include <quack/type.h>
#include <quack/lexer.h>
#include <quack/token.h>
#include <stdlib.h>
#include <stdbool.h>

static QkToken* advance(QkParser* parser);

static bool expect(QkParser* parser, QkTokKind kind);
#define EXPECT(parser, kind) do { if (!expect((parser), (kind))) return NULL; } while (0)

static QkStatement* parseAssign(QkParser* parser, QkExpression* name, QkType* possibleType);
static QkType* parseType(QkParser* parser);

QkParser qkParserInit(QkLexer* lexer, QkUnit* unit) {
    QkParser parser = (QkParser) {
        .lexer = lexer,
        .unit = unit,
    };

    parser.nextToken = qkLexerNext(parser.lexer);

    return parser;
}

QkStatement* qkParseStatement(QkParser* parser) {
    // Check for keywords like "fn" here

    QkExpression* expr = qkParseExpression(parser);
    if (expr == NULL) return NULL;

    switch (parser->nextToken.kind) {
        case QK_TOK_COL_EQ:
            advance(parser);
            return parseAssign(parser, expr, NULL);

        case QK_TOK_COL: {
            advance(parser);
            QkType* type = parseType(parser);
            EXPECT(parser, QK_TOK_EQ);
            return parseAssign(parser, expr, type);
        }
        
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

    qkDynArrPush(&parser->unit->errLog, qkErrorUnexpToken(parser->currentToken.loc, &parser->currentToken));
    return NULL;
}

static QkStatement* parseAssign(QkParser* parser, QkExpression* name, QkType* possibleType) {
    QkExpression* value = qkParseExpression(parser);
    if (value == NULL) return NULL;

    EXPECT(parser, QK_TOK_SEMI);

    QkStatement* stmt = malloc(sizeof(QkStatement));
    *stmt = (QkStatement) {
        .kind = QK_STMT_KIND_ASSIGN,
        .loc = name->loc,
        .valAssign.name = name,
        .valAssign.value = value,
        .valAssign.possibleType = possibleType,
    };
    return stmt;
}

static QkType* parseType(QkParser* parser) {
    i64 typeFlags = 0;

    if (parser->nextToken.kind == QK_TOK_IDENT) {
        if (qkStringArrEq(&parser->nextToken.valIdent, "const")) {
            typeFlags |= QK_TYPE_FLAG_CONST;
            advance(parser);
        }
    }

    if (parser->nextToken.kind == QK_TOK_STAR) {
        advance(parser);
        QkType* type = qkTypePtrInit(parseType(parser));
        type->typeFlags = typeFlags;
        return type;
    }

    if (parser->nextToken.kind == QK_TOK_IDENT) {
        if (qkStringArrEq(&parser->nextToken.valIdent, "Int8")) {
            QkType* type = qkTypeBasicInit(QK_BASIC_TYPE_I8);
            type->typeFlags = typeFlags;
            advance(parser);
            return type;
        } else if (qkStringArrEq(&parser->nextToken.valIdent, "Uint8")) {
            QkType* type = qkTypeBasicInit(QK_BASIC_TYPE_U8);
            type->typeFlags = typeFlags;
            advance(parser);
            return type;
        }
    }

    QkExpression* name = qkParseExpression(parser);
    if (name == NULL) return NULL;
    QkType* type = qkTypeIncompleteInit(name);
    type->typeFlags |= typeFlags;
    return type;
}

static QkToken* advance(QkParser* parser) {
    QkToken cur = parser->nextToken;
    parser->currentToken = cur;

    parser->nextToken = qkLexerNext(parser->lexer);

    return &parser->currentToken;
}

static bool expect(QkParser* parser, QkTokKind kind) {
    QkToken* token = advance(parser);
    if (token->kind != kind) {
        qkDynArrPush(&parser->unit->errLog, qkErrorExpToken(token->loc, kind, token));
        return false;
    }

    return true;
}
