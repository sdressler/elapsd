#ifndef ELAPSD_PARAMS_H
#define ELAPSD_PARAMS_H

#ifdef __cplusplus

#include <map>
#include <string>

namespace ENHANCE {

/**
 * This class contains the public interface to elaps'd. Internally, it acts as a
 * wrapper for all provided methods.
 *
 * \author    Sebastian Dreßler (ZIB)
 * \date      2012-05-24
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
