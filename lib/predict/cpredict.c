#include <elapsd/predict/predict.h>
#include <elapsd/predict/cpredict.h>

predict* predictInit(
    const char *db,
    const elapsdParams *p,
    int KernelID,
    int DeviceID
) {
    return cpp_callback_predictInit(db, p, KernelID, DeviceID);
}

int predictCreatePredictionModel_Lagrange(predict *p) {
    return cpp_callback_predictCreatePredictionModel_Lagrange(p);
}

float predictGetRuntimePrediction(predict *p, int x) {
    return cpp_callback_predictGetRuntimePrediction(p, x);
}
