#include "./parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <libquack/ast.h>
#include <libquack/dynArr.h>
#include <libquack/error.h>
#include <libquack/string.h>
#include <libquack/type.h>
#include <libquack/lexer.h>
#include <libquack/token.h>

static QkToken advance(QkParser* parser);

static bool expect(QkParser* parser, QkTokKind kind);
#define EXPECT(parser, kind) do { if (!expect((parser), (kind))) return NULL; } while (0)

// To be used like
//  QkStatement* TRY(stmt, qkParseStatement(parser))
#define TRY(name, value) name = value; if (name == NULL) return NULL

static QkStatement* parseAssign(QkParser* parser, QkExpression* name, QkType* possibleType);
static QkStatement* parseReassign(QkParser* parser, QkExpression* name);
static QkStatement* parseFunction(QkParser* parser);
static QkStatement* parseWhile(QkParser* parser);
static QkStatement* parseIf(QkParser* parser);
static QkStatement* parseStruct(QkParser* parser);

static QkExpression* parseBlock(QkParser* parser);
static QkExpression* parseTypedecl(QkParser* parser);
static QkType* parseType(QkParser* parser);

static QkFunSig* parseFunSig(QkParser* parser);

QkParser qkParserInit(QkLexer* lexer, QkUnit* unit) {
    QkParser parser;
    parser.lexer = lexer;
    parser.unit = unit;
    parser.nextToken = qkLexerNext(lexer);

    return parser;
}

QkStatement* qkParseStatement(QkParser* parser) {
    switch (parser->nextToken.kind) {
        case QK_TOK_FUN:
            advance(parser);
            return parseFunction(parser);
        case QK_TOK_WHILE:
	    advance(parser);
	    return parseWhile(parser);
        case QK_TOK_IF:
	    advance(parser);
	    return parseIf(parser);
        case QK_TOK_STRUCT:
	    advance(parser);
	    return parseStruct(parser);
        default: break;
    }

    QkExpression* TRY(expr, qkParseExpression(parser));

    switch (parser->nextToken.kind) {
        case QK_TOK_COL_EQ:
            advance(parser);
            return parseAssign(parser, expr, NULL);

        case QK_TOK_COL: {
            advance(parser);
	    QkType* TRY(type, parseType(parser));
            EXPECT(parser, QK_TOK_EQ);
            return parseAssign(parser, expr, type);
        }

        case QK_TOK_EQ:
	    advance(parser);
	    return parseReassign(parser, expr);
        
        default: {
            QkStatement* stmt = malloc(sizeof(QkStatement));
	    stmt->kind = QK_STMT_KIND_EXPR;
	    stmt->valExpr = expr;
	    
            return stmt;
        }
    }
}

QkExpression* qkParseExpression(QkParser* parser) {
    switch (advance(parser).kind) {
        case QK_TOK_IDENT: {
            QkExpression* e = malloc(sizeof(QkExpression));
            e->kind = QK_EXPR_KIND_IDENT;
	    e->loc = parser->currentToken.loc;
	    e->valString = parser->currentToken.valIdent;
            return e;
        }

        case QK_TOK_INT: {
            QkExpression* e = malloc(sizeof(QkExpression));
	    e->kind = QK_EXPR_KIND_INT_LIT;
	    e->loc = parser->currentToken.loc;
	    e->valIntLit = parser->currentToken.valInt;
            return e;
        }

        case QK_TOK_EOF: {
            QkExpression* e = malloc(sizeof(QkExpression));
            e->kind = QK_EXPR_KIND_EOF;
	    e->loc = parser->currentToken.loc;
            return e;
        }

        default: break;
    }

    qkDynArrPush(&parser->unit->errLog, qkErrorUnexpToken(parser->currentToken.loc, &parser->currentToken));
    return NULL;
}

static QkStatement* parseAssign(QkParser* parser, QkExpression* name, QkType* possibleType) {
    QkStatement* stmt = malloc(sizeof(QkStatement));
    stmt->kind = QK_STMT_KIND_ASSIGN;
    stmt->loc = name->loc;
    
    stmt->valAssign.name = name;
    stmt->valAssign.possibleType = possibleType;
    
    TRY(stmt->valAssign.value, qkParseExpression(parser));
    
    EXPECT(parser, QK_TOK_SEMI);
    return stmt;
}

static QkType* parseType(QkParser* parser) {
    i64 typeFlags = 0;

    if (parser->nextToken.kind == QK_TOK_CONST) {
	typeFlags |= QK_TYPE_FLAG_CONST;
	advance(parser);
    }

    if (parser->nextToken.kind == QK_TOK_STAR) {
        advance(parser);
        QkType* type = qkTypePtrInit(parseType(parser));
        type->typeFlags = typeFlags;
        return type;
    }

    QkExpression* TRY(name, qkParseExpression(parser));
    QkType* type = qkTypeIncompleteInit(name);
    type->typeFlags |= typeFlags;
    return type;
}

static QkExpression* parseBlock(QkParser* parser) {
    QkExpression* expr = malloc(sizeof(QkExpression));
    expr->kind = QK_EXPR_KIND_BLOCK;
    
    EXPECT(parser, QK_TOK_CURLY_OPEN);
    expr->loc = parser->currentToken.loc;
    
    expr->valBlock = qkDynArrInit(0);
    while (parser->nextToken.kind != QK_TOK_CURLY_CLOSE) {
	QkStatement* TRY(stmt, qkParseStatement(parser));
        qkDynArrPush(&expr->valBlock, stmt);
    }
    advance(parser);

    return expr;
}

static QkStatement* parseFunction(QkParser* parser) {
    QkStatement* stmt = malloc(sizeof(QkStatement));
    stmt->kind = QK_STMT_KIND_FUN;
    stmt->loc = parser->currentToken.loc;

    TRY(stmt->valFun.name, qkParseExpression(parser));
    TRY(stmt->valFun.sig, parseFunSig(parser));
    TRY(stmt->valFun.body, parseBlock(parser));

    return stmt;
}

static QkFunSig* parseFunSig(QkParser* parser) {
    QkFunSig* sig = malloc(sizeof(QkFunSig));
    sig->parameters = qkDynArrInit(0);
    
    EXPECT(parser, QK_TOK_PAREN_OPEN);
    while (true) {
	if (parser->nextToken.kind == QK_TOK_PAREN_CLOSE) {
	    advance(parser);
	    break;
	}
	
	QkExpression* TRY(parameter, parseTypedecl(parser));
	qkDynArrPush(&sig->parameters, parameter);
	
	if (parser->nextToken.kind == QK_TOK_COMMA) {
	    advance(parser);
	} else {
	    EXPECT(parser, QK_TOK_PAREN_CLOSE);
	    break;
	}
    }

    EXPECT(parser, QK_TOK_THIN_ARROW);
    sig->returnType = parseType(parser);
    
    return sig;
}

static QkStatement* parseReassign(QkParser* parser, QkExpression* name) {
    QkStatement* stmt = malloc(sizeof(QkStatement));
    stmt->kind = QK_STMT_KIND_REASSIGN;
    stmt->loc = name->loc;
    stmt->valReassign.name = name;
    
    TRY(stmt->valReassign.value, qkParseExpression(parser));
    EXPECT(parser, QK_TOK_SEMI);

    return stmt;
}

static QkStatement* parseWhile(QkParser* parser) {
    QkStatement* stmt = malloc(sizeof(QkStatement));
    stmt->kind = QK_STMT_KIND_WHILE;
    stmt->loc = parser->currentToken.loc;

    TRY(stmt->valWhile.condition, qkParseExpression(parser));
    TRY(stmt->valWhile.body, parseBlock(parser));

    return stmt;
}

static QkStatement* parseIf(QkParser* parser) {
    QkStatement* stmt = malloc(sizeof(QkStatement));
    stmt->kind = QK_STMT_KIND_IF;
    stmt->loc = parser->currentToken.loc;

    TRY(stmt->valIf.condition, qkParseExpression(parser));
    TRY(stmt->valIf.body, parseBlock(parser));

    if (parser->nextToken.kind == QK_TOK_ELSE) {
	advance(parser);
	TRY(stmt->valIf.elseBody, parseBlock(parser));
    } else {
	stmt->valIf.elseBody = NULL;
    }

    return stmt;
}

static QkExpression* parseTypedecl(QkParser* parser) {
    QkExpression* expr = malloc(sizeof(QkExpression));
    expr->kind = QK_EXPR_KIND_TYPEDECL;
    expr->loc = parser->currentToken.loc;

    TRY(expr->valTypedecl.name, qkParseExpression(parser));
    EXPECT(parser, QK_TOK_COL);
    TRY(expr->valTypedecl.type, parseType(parser));

    return expr;
}

static QkStatement* parseStruct(QkParser *parser) {
    // TODO: Implement
}

static QkToken advance(QkParser* parser) {
    QkToken cur = parser->nextToken;
    parser->currentToken = cur;
    
    parser->nextToken = qkLexerNext(parser->lexer);

    return parser->currentToken;
}

static bool expect(QkParser* parser, QkTokKind kind) {
    QkToken token = advance(parser);
    if (token.kind != kind) {
        qkDynArrPush(&parser->unit->errLog, qkErrorExpToken(token.loc, kind, &token));
        return false;
    }

    return true;
}
