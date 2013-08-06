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

int predictGetNumberOfDistinctMeasurements(predict *p) {
    return cpp_callback_predictGetNumberOfDistinctMeasurements(p);
}

int predictGenerateLagrangePolynomial(predict *p) {
    return cpp_callback_predictGenerateLagrangePolynomial(p);
}

double predictMakeRuntimePrediction(predict *p, int N) {
    return cpp_callback_predictMakeRuntimePrediction(p, N);
}

