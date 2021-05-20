#include "./lexer.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <libquack/location.h>
#include <libquack/token.h>
#include <libquack/dynArr.h>
#include <libquack/string.h>

#define PEEK(l) (l)->unit->source.data[(l)->index + 1]

inline static bool canIgnore(char c);
inline static char advance(QkLexer* l);
inline static QkLocation getLocation(QkLexer* lexer);

static QkToken getIdent(QkLexer* lexer);
static QkToken getNumber(QkLexer* lexer);

QkLexer qkLexerInit(QkUnit* unit) {
    QkLexer lexer;
    lexer.unit = unit;
    lexer.index = 0;
    lexer.cur = unit->source.data[0];
    lexer.line = 1;
    lexer.col = 1;
    lexer.lineIndex = 0;

    return lexer;
}

QkToken qkLexerNext(QkLexer* lexer) {
    while (canIgnore(lexer->cur)) advance(lexer);

    if (lexer->index >= lexer->unit->source.size) return QK_TOKEN(QK_TOK_EOF, getLocation(lexer));
    else if (isalpha(lexer->cur)) return getIdent(lexer);
    else if (isdigit(lexer->cur)) return getNumber(lexer);
    else {
        QkLocation loc = getLocation(lexer);
        switch (lexer->cur) {
            case ':':
                switch (advance(lexer)) {
                    case '=': advance(lexer); return QK_TOKEN(QK_TOK_COL_EQ, loc);
                    default: return QK_TOKEN(QK_TOK_COL, loc);
                }
            
            case ';':
                advance(lexer);
                return QK_TOKEN(QK_TOK_SEMI, loc);

            case '*':
                advance(lexer);
                return QK_TOKEN(QK_TOK_STAR, loc);

            case '=':
                advance(lexer);
                return QK_TOKEN(QK_TOK_EQ, loc);

            case '{':
                advance(lexer);
                return QK_TOKEN(QK_TOK_CURLY_OPEN, loc);

            case '}':
                advance(lexer);
                return QK_TOKEN(QK_TOK_CURLY_CLOSE, loc);

	    case '(':
		advance(lexer);
		return QK_TOKEN(QK_TOK_PAREN_OPEN, loc);

	    case ')':
		advance(lexer);
		return QK_TOKEN(QK_TOK_PAREN_CLOSE, loc);

	    case '-':
		switch (advance(lexer)) {
		    case '>': advance(lexer); return QK_TOKEN(QK_TOK_THIN_ARROW, loc);
		}
		break;

	    case ',':
		advance(lexer);
		return QK_TOKEN(QK_TOK_COMMA, loc);
        }
    }

    qkDynArrPush(&lexer->unit->errLog, qkErrorUnexpChar(getLocation(lexer), lexer->cur));

    return QK_TOKEN(QK_TOK_NULL, getLocation(lexer));
}



static QkToken getIdent(QkLexer* lexer) {
    QkToken token;
    token.loc = getLocation(lexer);

    i64 start = lexer->index;
    while (isalnum(lexer->cur)) advance(lexer);

    QkString ident = qkSliceString(&lexer->unit->source, start, lexer->index);

    if (qkStringArrEq(&ident, "fun")) token.kind = QK_TOK_FUN;
    else if (qkStringArrEq(&ident, "const")) token.kind = QK_TOK_CONST;
    else if (qkStringArrEq(&ident, "if")) token.kind = QK_TOK_IF;
    else if (qkStringArrEq(&ident, "else")) token.kind = QK_TOK_ELSE;
    else if (qkStringArrEq(&ident, "while")) token.kind = QK_TOK_WHILE;
    else if (qkStringArrEq(&ident, "struct")) token.kind = QK_TOK_STRUCT;
    else token.kind = QK_TOK_IDENT;

    token.valIdent = ident;

    return token;
}

static QkToken getNumber(QkLexer* lexer) {
    QkToken token;
    token.kind = QK_TOK_INT;
    token.loc = getLocation(lexer);

    i64 start = lexer->index;
    while (isdigit(lexer->cur)) advance(lexer);  // TODO: Add support for floats
    
    token.valInt = strtol(qkSliceString(
			    &lexer->unit->source,
			    start, lexer->index).data,
			    NULL, 10);

    return token;
}

inline static QkLocation getLocation(QkLexer* lexer) {
    QkLocation location;
    location.line = lexer->line;
    location.col = lexer->col;
    location.index = lexer->index;
    location.lineIndex = lexer->lineIndex;
    
    return location;
}

inline static char advance(QkLexer* lexer) {
    if (lexer->cur == '\n') {
        lexer->line += 1;

        // This is to omit the newline, the line itself doesn't really contain it
        lexer->lineIndex = lexer->index + 1;

        // We set it to 0, not 1, because the "first" one is actually the newline,
        //  which we ignored on the line above
        lexer->col = 0;
    }

    lexer->index += 1;
    lexer->cur = lexer->unit->source.data[lexer->index];
    lexer->col += 1;

    return lexer->cur;
}

inline static bool canIgnore(char c) {
    return
        (c == '\n') ||
        (c == '\b') ||
        (c == '\r') ||
	(c == '\t') ||
        (c == ' ')
    ;
}
