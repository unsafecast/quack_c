#include "./ast.h"
#include <libquack/type.h>
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
            fputs("): ", stream);
            qkPrintType(stmt->valAssign.possibleType, stream);
            fputs("\n", stream);
            break;

        case QK_STMT_KIND_EXPR:
            fputs("SExpression(", stream);
            qkPrintExpression(0, stmt->valExpr, stream);
            fputs(")\n", stream);
            break;

        case QK_STMT_KIND_FUN:
            fputs("SFun ", stream);
            qkPrintExpression(0, stmt->valFun.name, stream);
            fputs(": ", stream);
	    qkPrintFunSig(stmt->valFun.sig, stream);
	    fputc('\n', stream);
	    qkPrintExpression(offset + 2, stmt->valFun.body, stream);
            break;

        case QK_STMT_KIND_REASSIGN:
	    fputs("SReassign(", stream);
	    qkPrintExpression(0, stmt->valReassign.name, stream);
	    fputs(" = ", stream);
	    qkPrintExpression(0, stmt->valReassign.value, stream);
	    fputs(")\n", stream);
	    break;

        case QK_STMT_KIND_WHILE:
	    fputs("SWhile(", stream);
	    qkPrintExpression(0, stmt->valWhile.condition, stream);
	    fputs(")\n", stream);
	    qkPrintExpression(offset + 2, stmt->valWhile.body, stream);
	    break;

	case QK_STMT_KIND_IF:
	    fputs("SIf(", stream);
	    qkPrintExpression(0, stmt->valIf.condition, stream);
	    fputs(")\n", stream);
	    qkPrintExpression(offset + 2, stmt->valIf.body, stream);
	    if (stmt->valIf.elseBody != NULL) {
		fputs("SElse\n", stream);
		qkPrintExpression(offset + 2, stmt->valIf.elseBody, stream);
	    }
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
            fprintf(stream, "EIntLit(%ld)", expr->valIntLit);
            break;

        case QK_EXPR_KIND_BLOCK: {
	    // This is a weird hack, but it's so we don't offset 2 times
	    //  the **first** time we print
	    i64 realOffset = 0;
	    QK_FOR(&expr->valBlock) {
		qkPrintStatement(realOffset, expr->valBlock.data[it], stream);
		realOffset = offset;
	    }
	    break;
	}

        default:
            fputs("<unimplemented print for expression>", stream);
            break;
    }
}

void qkPrintFunSig(const QkFunSig* sig, FILE* stream) {
    fputs("fun (", stream);
    QK_FOR(&sig->parameterNames) {
	qkPrintExpression(0, sig->parameterNames.data[it], stream);
	fputs(": ", stream);
	qkPrintType(sig->parameterTypes.data[it], stream);
	fputs(", ", stream);
    }
    fputs(") -> ", stream);
    qkPrintType(sig->returnType, stream);
}

inline static void printOffset(i64 offset, FILE* stream) {
    while (offset-- > 0) {
        fputc(' ', stream);
    }
}
