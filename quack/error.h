#pragma once
#include <stdio.h>
#include <quack/location.h>
#include <quack/string.h>
#include <quack/token.h>

typedef enum QkErrKind QkErrKind;
enum QkErrKind {
    QK_ERR_UNEXP_CHAR,
    QK_ERR_EXP_TOKEN,
    QK_ERR_UNEXP_TOKEN,
};

typedef struct QkError QkError;
struct QkError {
    QkErrKind kind;
    QkLocation location;

    union {
        char valUnexpChar;
        QkToken unexpectedToken;

        struct {
            QkTokKind expected;
            QkToken got;
        } valExpToken;
    };
};

// We're only gonna need errors on the heap
QkError* qkErrorUnexpChar(QkLocation loc, char c);
QkError* qkErrorExpToken(QkLocation loc, QkTokKind expected, const QkToken* got);
QkError* qkErrorUnexpToken(QkLocation loc, const QkToken* token);

void qkPrintError(const QkError* err, const QkString* source, FILE* stream);
