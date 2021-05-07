#include "quack/ast.h"
#include "quack/error.h"
#include <quack/string.h>
#include <quack/lexer.h>
#include <quack/token.h>
#include <quack/unit.h>
#include <quack/parser.h>
#include <string.h>
#include <quack/io.h>

int main() {
    QkString source = qkReadWholeFile("../examples/assign.qk");

    QkUnit unit = qkUnitInit();
    unit.source = source;

    QkLexer lexer = qkLexerInit(&unit);
    QkParser parser = qkParserInit(&lexer, &unit);

    fprintf(stderr, "\n");
    QkStatement* stmt = qkParseStatement(&parser);
    while ((stmt != NULL) && (stmt->valExpr->kind != QK_EXPR_KIND_EOF)) {
        qkPrintStatement(0, stmt, stderr);
        stmt = qkParseStatement(&parser);
    }

    fprintf(stderr, "\n");
    QK_FOR(&unit.errLog) {
        qkPrintError(unit.errLog.data[it], &unit.source, stderr);
    }
}
