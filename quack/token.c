#include "./token.h"
#include <quack/string.h>

void qkPrintToken(QkToken* tok, FILE* stream) {
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
    }
}
