#include "../include/elapsd/cparams.h"
#include "../include/elapsd/Params.h"

elapsdParams* elapsdParamsInit() {
    return cpp_callback_elapsdParamsInit();
}

int elapsdParamsAddParam(elapsdParams *p, const char *key, int value) {
    return cpp_callback_elapsdParamsAddParam(p, key, value);
}
