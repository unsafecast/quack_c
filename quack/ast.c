#include "./ast.h"
#include <stdio.h>

inline static void printOffset(i64 offset, FILE* stream);

void qkPrintStatement(i64 offset, const QkStatement* stmt, FILE* stream) {
    printOffset(offset, stream);
    switch (stmt->kind) {
        case QK_STMT_KIND_ASSIGN:
            fputs("SAssign(", stream);
            qkPrintExpression(0, stmt->valAssign.name, stream);
            fputs(" := ", stream);
            qkPrintExpression(0, stmt->valAssign.value, stream);
            fputs(")\n", stream);
            break;

        case QK_STMT_KIND_EXPR:
            fputs("SExpression(", stream);
            qkPrintExpression(0, stmt->valExpr, stream);
            fputs(")\n", stream);
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
            fputs("EIdent(", stream);
            qkPrintString(&expr->valString, stream);
            fputs(")", stream);
            break;

        case QK_EXPR_KIND_INT_LIT:
            fprintf(stream, "EIntLit(%lld)", expr->valIntLit);
            break;

        default:
            fputs("<unimplemented print for expression>", stream);
            break;
    }
}

inline static void printOffset(i64 offset, FILE* stream) {
    while (offset-- > 0) {
        fputc(' ', stream);
    }
}
