#include "./unit.h"
#include <libquack/dynArr.h>

QkUnit qkUnitInit() {
    return (QkUnit) {
        .errLog = qkDynArrInit(0),
    };
}
