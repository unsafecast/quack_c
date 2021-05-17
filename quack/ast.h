#pragma once
#include <quack/string.h>
#include <quack/integer.h>
#include <quack/location.h>
#include <stdio.h>
#include <quack/type.h>
#include <quack/dynArr.h>

typedef enum QkExpressionKind QkExpressionKind;
enum QkExpressionKind {
    QK_EXPR_KIND_IDENT,
    QK_EXPR_KIND_INT_LIT,
    QK_EXPR_KIND_EOF,
    QK_EXPR_KIND_BLOCK,
};

typedef struct QkExpression QkExpression;
typedef struct QkStatement QkStatement;

struct QkExpression {
    QkExpressionKind kind;
    QkLocation loc;

    union {
        QkString valString;
        i64 valIntLit;
        QkDynArr valBlock; // Of QkStatement*
    };
};

typedef enum QkStatementKind QkStatementKind;
enum QkStatementKind {
    QK_STMT_KIND_ASSIGN,
    QK_STMT_KIND_EXPR,

    // NOTE: When we implement lambdas, this will probably move to the QkExpression structure
    QK_STMT_KIND_FUN,
};

struct QkStatement {
    QkStatementKind kind;
    QkLocation loc;

    union {
        struct {
            QkExpression* name;
            QkExpression* value;
            QkType* possibleType;
        } valAssign;

        struct {
            // TODO: Add parameters
            QkExpression* name;
            QkExpression* body;
        } valFun;

        QkExpression* valExpr;
    };
};

void qkPrintStatement(i64 offset, const QkStatement* stmt, FILE* stream);
void qkPrintExpression(i64 offset, const QkExpression* expr, FILE* stream);
