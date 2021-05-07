#include "./ast.h"
#include <stdio.h>

inline static void printOffset(i64 offset, FILE* stream);

void qkPrintStatement(i64 offset, const QkStatement* stmt, FILE* stream) {
    printOffset(offset, stream);
    switch (stmt->kind) {
        case QK_STMT_KIND_ASSIGN:
            fputs("StmtAssign\n", stream);

            printOffset(offset + 2, stream);
            fputs("├ Name: ", stream);
            qkPrintExpression(0, stmt->valAssign.name, stream);

            printOffset(offset + 2, stream);
            fputs("└ Value: ", stream);
            qkPrintExpression(0, stmt->valAssign.value, stream);
            break;

        case QK_STMT_KIND_EXPR:
            fputs("StmtExpression ", stream);
            qkPrintExpression(0, stmt->valExpr, stream);
            break;

        default:
            fputs("<unimplemented print for statement>\n", stream);
            break;
    }
}

void qkPrintExpression(i64 offset, const QkExpression* expr, FILE* stream) {
    printOffset(offset, stream);
    switch (expr->kind) {
        case QK_EXPR_KIND_IDENT:
            fputs("ExprIdent(", stream);
            qkPrintString(&expr->valString, stream);
            fputs(")\n", stream);
            break;

        case QK_EXPR_KIND_INT_LIT:
            fprintf(stream, "ExprIntLit(%lld)\n", expr->valIntLit);
            break;

        default:
            fputs("<unimplemented print for expression>\n", stream);
            break;
    }
}

inline static void printOffset(i64 offset, FILE* stream) {
    while (offset-- > 0) {
        fputc(' ', stream);
    }
}
