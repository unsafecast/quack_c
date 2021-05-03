#include "quack/string.h"
#include <quack/lexer.h>
#include <quack/token.h>
#include <quack/unit.h>
#include <string.h>

int main() {
    char* source = "x := 1;";

    QkUnit unit;
    qkStringFromArray(&unit.source, source, strlen(source));

    QkLexer lexer;
    qkLexerInit(&lexer, &unit);
}
