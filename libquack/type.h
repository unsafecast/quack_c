#pragma once
#include <stdio.h>
#include <libquack/string.h>
#include <libquack/integer.h>

typedef enum QkTypeKind QkTypeKind;
enum QkTypeKind {
    QK_TYPE_KIND_BASIC,
    QK_TYPE_KIND_PTR,
    QK_TYPE_KIND_INCOMPLETE,
};

typedef enum QkTypeFlag QkTypeFlag;
enum QkTypeFlag {
    QK_TYPE_FLAG_CONST = (1 << 0),
};

typedef enum QkBasicType QkBasicType;
enum QkBasicType {
    QK_BASIC_TYPE_I8,
    QK_BASIC_TYPE_U8
};

typedef struct QkType QkType;
typedef struct QkExpression QkExpression;
struct QkType {
    QkTypeKind kind;
    i64 typeFlags;

    union {
        QkBasicType valBasic;
        QkType* valPtr;
        QkExpression* valIncomplete;
    };
};

QkType* qkTypeBasicInit(QkBasicType basicType);
QkType* qkTypePtrInit(QkType* base);
QkType* qkTypeIncompleteInit(QkExpression* expression);

void qkPrintType(const QkType* type, FILE* stream);
