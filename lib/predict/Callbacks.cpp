#include <elapsd/predict/predict.h>
#include <elapsd/predict/cpredict.h>
#include <elapsd/cparams.h>

#include <elapsd/Errors.h>

using namespace ENHANCE;

predict* cpp_callback_predictInit(
    const char *db, const elapsdParams *p, int KernelID, int DeviceID
) {

    predict *p_;

    try {
    
        p_ = new predict(std::string(db), p->getKeys(), KernelID, DeviceID);

    } catch (...) {

        return NULL;

    }

    return p_;

}

int cpp_callback_predictGetNumberOfDistinctMeasurements(predict *p) {
    return p->getNumberOfDistinctMeasurements();
}

int cpp_callback_predictGenerateLagrangePolynomial(predict *p) {

    try {
        p->generateLagrangePolynomial();
    } catch (...) {
        return E_NOK; //TODO: THIS NEEDS TO BE CHANGED!!!
    }

    return E_OK;

}

double cpp_callback_predictMakeRuntimePrediction(predict *p, int N) {
    return p->makeRuntimePrediction(N);
}

