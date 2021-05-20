#include "./unit.h"
#include <libquack/dynArr.h>

QkUnit qkUnitInit() {
    QkUnit unit;
    unit.errLog = qkDynArrInit(0);

    return unit;
}
