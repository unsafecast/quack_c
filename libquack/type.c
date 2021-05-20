#include "./type.h"
#include <stdlib.h>
#include <libquack/ast.h>

QkType* qkTypeBasicInit(QkBasicType basicType) {
    QkType* type = malloc(sizeof(QkType));
    type->kind = QK_TYPE_KIND_BASIC;
    type->valBasic = basicType;

    return type;
}

QkType* qkTypePtrInit(QkType* base) {
    QkType* type = malloc(sizeof(QkType));
    type->kind = QK_TYPE_KIND_PTR;
    type->valPtr = base;
    
    return type;
}

QkType* qkTypeIncompleteInit(QkExpression* expression) {
    QkType* type = malloc(sizeof(QkType));
    type->kind = QK_TYPE_KIND_INCOMPLETE;
    type->valIncomplete = expression;

    return type;
}

#define SWP(variant, message) case variant: fputs(message, stream); break;
void qkPrintType(const QkType* type, FILE* stream) {
    if (type == NULL) {
        fputs("notype", stream);
        return;
    }

    if (type->typeFlags & QK_TYPE_FLAG_CONST) fputs("const ", stream);

    switch (type->kind) {
        case QK_TYPE_KIND_BASIC:
            switch (type->valBasic) {
                SWP(QK_BASIC_TYPE_I8, "Int8");
                SWP(QK_BASIC_TYPE_U8, "Uint8");
            }
            break;
        
        case QK_TYPE_KIND_PTR:
            fputc('*', stream);
            qkPrintType(type->valPtr, stream);
            break;

        case QK_TYPE_KIND_INCOMPLETE:
            fputs("incomplete ", stream);
            qkPrintExpression(0, type->valIncomplete, stream);
            break;
    }
}
#undef SWP
