/**
 *
 * \mainpage test
 *
 * C wrapper for elapsd (which is written in C++).
 *
 * @author	Sebsatian Dressler
 * @date	2012-05-25
 *
 * */

#ifndef C_PREDICT_H_INC_
#define C_PREDICT_H_INC_

#include "predict.h"

#include "../cparams.h"

#ifdef __cplusplus

using namespace ENHANCE;

extern "C" {
#endif

predict* predictInit(
    const char *db, const elapsdParams *p, int KernelID, int DeviceID
);
predict* cpp_callback_predictInit(
    const char *db, const elapsdParams *p, int KernelID, int DeviceID
);

int predictCreatePredictionModel_Lagrange(predict *p);
int cpp_callback_predictCreatePredictionModel_Lagrange(predict *p);

float predictGetRuntimePrediction(predict *p, int x);
float cpp_callback_predictGetRuntimePrediction(predict *p, int x);

#ifdef __cplusplus
}
#endif

#endif
