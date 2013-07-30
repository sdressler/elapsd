#ifndef ELAPSD_PARAMS_H
#define ELAPSD_PARAMS_H

#ifdef __cplusplus

#include <map>
#include <string>

#include "SQLite.h"

namespace ENHANCE {

/**
 *
 * This class provides an abstraction layer to the key/value data structure used
 * to store parameters. Its main purpose is to enable a C/FORTRAN interface.
 *
 * \author    Sebastian Dreßler (ZIB)
 * \date      2013-07-23
 * \version   0.1
 * \copyright BSD License
 *
 * */
class elapsdParams : public std::map<std::string, int> {

};
}

#else
typedef struct elapsdParams elapsdParams;
#endif

#endif /* ELAPSD_H */
