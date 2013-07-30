/**
 *
 * C wrapper for std::map
 *
 * @author	Sebsatian Dressler
 * @date	2013-07-23
 *
 * */

#ifndef C_MAP_H
#define C_MAP_H

#include "Params.h"

#ifdef __cplusplus

using namespace ENHANCE;

extern "C" {
#endif

elapsdParams* elapsdParamsInit();
elapsdParams* cpp_callback_elapsdParamsInit();

int elapsdParamsAddParam(elapsdParams *p, const char *key, int value);
int cpp_callback_elapsdParamsAddParam(elapsdParams *p, const char *key, int value);

#ifdef __cplusplus
}
#endif

#endif
