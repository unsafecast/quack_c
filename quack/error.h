#pragma once
#include <stdio.h>
#include <quack/location.h>
#include <quack/string.h>

typedef enum QkErrKind QkErrKind;
enum QkErrKind {
    QK_ERR_UNEXP_CHAR,
};

typedef struct QkError QkError;
struct QkError {
    QkErrKind kind;
    QkLocation location;

    union {
        char val_unexp_char;
    };
};

// We're only gonna need errors on the heap
QkError* qkErrorUnexpChar(QkLocation loc, char c);

void qkPrintError(const QkError* err, const QkString* source, FILE* stream);
