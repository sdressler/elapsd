/**
 *
 * C wrapper for std::map
 *
 * @author	Sebsatian Dressler
 * @date	2013-07-23
 *
 * */

#include "Errors.h"
#include "params.h"

#ifndef C_MAP_H
#define C_MAP_H

#ifdef __cplusplus

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
