#include <string.h>
#include <libquack/ast.h>
#include <libquack/error.h>
#include <libquack/string.h>
#include <libquack/lexer.h>
#include <libquack/token.h>
#include <libquack/unit.h>
#include <libquack/parser.h>
#include <libquack/io.h>

int main() {
    QkString source = qkReadWholeFile("../examples/assign.qk");

    QkUnit unit = qkUnitInit();
    unit.source = source;

    QkLexer lexer = qkLexerInit(&unit);
    QkParser parser = qkParserInit(&lexer, &unit);

    fprintf(stderr, "\n");
    QkStatement* stmt = qkParseStatement(&parser);
    while ((stmt != NULL) && parser.currentToken.kind != QK_TOK_EOF) {
	qkPrintStatement(0, stmt, stderr);
        stmt = qkParseStatement(&parser);
    }

    fprintf(stderr, "\n");
    QK_FOR(&unit.errLog) {
        qkPrintError(unit.errLog.data[it], &unit.source, stderr);
    }
}
