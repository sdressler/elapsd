#include <elapsd/predict/predict.h>
#include <elapsd/predict/cpredict.h>
#include <elapsd/cparams.h>

#include <elapsd/Errors.h>

#include <elapsd/predict/LagrangePredictionModel.h>

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

int cpp_callback_predictCreatePredictionModel_Lagrange(predict *p) {

    try {
        p->createPredictionModel<LagrangePredictionModel>();
    } catch (std::invalid_argument &e) {
        return E_PNTS;
    }

    return E_OK;

}

float cpp_callback_predictGetRuntimePrediction(predict *p, int x) {
    return p->getRuntimePrediction(x);
}
