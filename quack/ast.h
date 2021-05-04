#pragma once
#include <quack/string.h>
#include <quack/integer.h>
#include <quack/location.h>
#include <stdio.h>

typedef enum QkExpressionKind QkExpressionKind;
enum QkExpressionKind {
    QK_EXPR_KIND_IDENT,
    QK_EXPR_KIND_INT_LIT,
    QK_EXPR_KIND_EOF,
};

typedef struct QkExpression QkExpression;
struct QkExpression {
    QkExpressionKind kind;
    QkLocation loc;

    union {
        QkString valString;
        i64 valIntLit;
    };
};

typedef enum QkStatementKind QkStatementKind;
enum QkStatementKind {
    QK_STMT_KIND_ASSIGN,
    QK_STMT_KIND_EXPR,
};

typedef struct QkStatement QkStatement;
struct QkStatement {
    QkStatementKind kind;
    QkLocation loc;

    union {
        struct {
            QkExpression* name;
            QkExpression* value;
        } valAssign;

        QkExpression* valExpr;
    };
};

void qkPrintStatement(i64 offset, const QkStatement* stmt, FILE* stream);
void qkPrintExpression(i64 offset, const QkExpression* expr, FILE* stream);
