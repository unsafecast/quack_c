#include "./token.h"
#include "quack/location.h"
#include <quack/string.h>
#include <stdio.h>

void qkPrintToken(const QkToken* tok, FILE* stream) {
    qkPrintLocation(&tok->loc, stream);
    fputs(": ", stream);

    switch (tok->kind) {
        case QK_TOK_NULL:
            fputs("TokNull\n", stream);
            break;
        
        case QK_TOK_EOF:
            fputs("TokEOF\n", stream);
            break;

        case QK_TOK_IDENT:
            fputs("TokIdent(", stream);
            qkPrintString(&tok->val_ident, stream);
            fputs(")\n", stream);
            break;
        
        case QK_TOK_INT:
            fprintf(stream, "TokInt(%lld)\n", tok->val_int);
            break;

        case QK_TOK_CO_LEQ:
            fputs("TokColEq\n", stream);
            break;

        case QK_TOK_COL:
            fputs("TokCol\n", stream);
            break;

        case QK_TOK_SEMI:
            fputs("TokSemi\n", stream);
            break;
    }
}
