#include "./token.h"
#include <stdio.h>
#include <libquack/location.h>
#include <libquack/string.h>

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
            qkPrintString(&tok->valIdent, stream);
            fputs(")\n", stream);
            break;
        
        case QK_TOK_INT:
            fprintf(stream, "TokInt(%lld)\n", tok->valInt);
            break;

        case QK_TOK_COL_EQ:
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
