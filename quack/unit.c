#include "./unit.h"
#include "quack/dynArr.h"

QkUnit qkUnitInit() {
    return (QkUnit) {
        .errLog = qkDynArrInit(0),
    };
}
