#pragma once

#include <stdio.h>
#include <libquack/string.h>
#include <libquack/integer.h>
#include <libquack/location.h>
#include <libquack/type.h>
#include <libquack/dynArr.h>

typedef struct QkFunSig QkFunSig;
struct QkFunSig {
    QkDynArr parameterTypes;  // Of QkType*
    QkDynArr parameterNames;  // Of QkExpression*
    QkType* returnType;
};

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

    QK_STMT_KIND_REASSIGN,
    QK_STMT_KIND_WHILE,
    QK_STMT_KIND_IF,
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
	    QkFunSig* sig;
            QkExpression* name;
            QkExpression* body;
        } valFun;

        QkExpression* valExpr;

	struct {
	    QkExpression* name;
	    QkExpression* value;
	} valReassign;

	struct {
	    QkExpression* condition;
	    QkExpression* body;  // FIXME: Maybe this needs to be a separate structure?
	} valWhile;

	struct {
	    QkExpression* condition;
	    QkExpression* body;
	    QkExpression* elseBody;
	} valIf;
    };
};

void qkPrintStatement(i64 offset, const QkStatement* stmt, FILE* stream);
void qkPrintExpression(i64 offset, const QkExpression* expr, FILE* stream);
void qkPrintFunSig(const QkFunSig* sig, FILE* stream);
