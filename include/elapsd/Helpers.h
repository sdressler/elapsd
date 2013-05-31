/*
 * elapsdHelpers.h
 *
 *  Created on: Apr 19, 2013
 *      Author: bzcdress
 */

#ifndef ELAPSED_HELPERS_H_
#define ELAPSED_HELPERS_H_

#ifdef DEBUG
    #include <iostream>
    #define \
        DMSG(m) \
        _Pragma("omp critical") \
        std::cerr << "elapsed: " << m << "\n";
#else
    #define DMSG(m)
#endif

#endif /* ELAPSED_HELPERS_H_ */
