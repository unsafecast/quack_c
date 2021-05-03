#include "./error.h"
#include <stdio.h>
#include <stdlib.h>

QkError* qkErrorUnexpChar(char c) {
    QkError* e = malloc(sizeof(QkError));
    e->kind = QK_ERR_UNEXP_CHAR;
    e->val_unexp_char = c;
    
    return e;
}

// TODO: Make this a good error printer (with source code and stuff)
void qkPrintError(QkError* err, FILE* stream) {
    switch (err->kind) {
        case QK_ERR_UNEXP_CHAR:
            fprintf(stream, "Unexpected character '%c'\n", err->val_unexp_char);
            break;
    }
}
