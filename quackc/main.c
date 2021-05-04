#include "quack/error.h"
#include <quack/string.h>
#include <quack/lexer.h>
#include <quack/token.h>
#include <quack/unit.h>
#include <string.h>

int main() {
    char* source = "x := cr=ateArray[Something]();";

    QkUnit unit = qkUnitInit();
    unit.source = qkStringFromArray(source, strlen(source));

    QkLexer lexer = qkLexerInit(&unit);

    QkToken tok = qkLexerNext(&lexer);
    while ((tok.kind != QK_TOK_NULL) && (tok.kind != QK_TOK_EOF)) {
        qkPrintToken(&tok, stderr);
        tok = qkLexerNext(&lexer);
    }

    fprintf(stderr, "\n");
    QK_FOR(&unit.errLog) {
        qkPrintError(unit.errLog.data[it], &unit.source, stderr);
    }
}
