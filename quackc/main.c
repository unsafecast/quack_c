#include "quack/error.h"
#include <quack/string.h>
#include <quack/lexer.h>
#include <quack/token.h>
#include <quack/unit.h>
#include <string.h>

int main() {
    char* source = "x := 1;\n";

    QkUnit unit = qkUnitInit();
    unit.source = qkStringFromArray(source, strlen(source));

    QkLexer lexer = qkLexerInit(&unit);

    QkToken tok = qkLexerNext(&lexer);
    while (tok.kind != QK_TOK_NULL) {
        qkPrintToken(&tok, stdout);
        tok = qkLexerNext(&lexer);
    }
}
