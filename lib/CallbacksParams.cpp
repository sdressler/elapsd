#include <elapsd/Params.h>
#include <elapsd/cparams.h>

#include "../include/elapsd/Errors.h"

#include <stdexcept>
#include <iostream>

using namespace ENHANCE;

elapsdParams* cpp_callback_elapsdParamsInit() {

    elapsdParams *p;

    try {
        p = new elapsdParams();
    } catch (...) {
        return NULL;
    }

    return p;

}

int cpp_callback_elapsdParamsAddParam(
    elapsdParams *p,
    const char *key,
    int value
) {
    
    try {
        p->insert(std::make_pair(std::string(key), value));
    } catch (std::runtime_error &e) {
        return E_PMAP;
    }

    return E_OK;

}

