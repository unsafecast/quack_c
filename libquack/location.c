#include "./location.h"

void qkPrintLocation(const QkLocation* loc, FILE* stream) {
    fprintf(stream, "%lld:%lld", loc->line, loc->col);
}
