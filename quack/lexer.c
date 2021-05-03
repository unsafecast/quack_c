#include "./lexer.h"

void qkLexerInit(QkLexer* lexer, QkUnit* unit) {
    lexer->unit = unit;
    lexer->index = 0;
    lexer->cur = unit->source.data[0];
}
