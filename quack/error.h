#pragma once
#include <stdio.h>

typedef enum QkErrKind QkErrKind;
enum QkErrKind {
    QK_ERR_UNEXP_CHAR,
};

typedef struct QkError QkError;
struct QkError {
    QkErrKind kind;

    union {
        char val_unexp_char;
    };
};

// We're only gonna need errors on the heap
QkError* qkErrorUnexpChar(char c);

void qkPrintError(QkError* err, FILE* stream);
