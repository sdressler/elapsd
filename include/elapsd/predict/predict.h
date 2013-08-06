#ifndef PREDICT_H_INC_
#define PREDICT_H_INC_

#ifdef __cplusplus

#include <elapsd/Params.h>
#include <elapsd/predict/Lagrange.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <sqlite3.h>

/**
 * This is the main predictor class
 * */

namespace ENHANCE {

class predict {
    
private:
    boost::shared_ptr<sqlite3> db_;
    std::vector<std::string> parameters_;

    int KernelID_;
    int DeviceID_;

    Lagrange<3, int, double> L;

    /**
     * This routine checks, whether the provided parameters match those from the
     * DB. That is, their names are matched against the *experiment_parameters*
     * table of the DB. Exceptions are thrown on mismatches.
     * */
    bool checkMatchingParamsInDB();

public:
    /**
     * This replaces the default constructor, because we really need a DB which
     * is opened during object construction
     * */
    predict(
        const std::string &dbname,
        const std::vector<std::string> &parameters,
        const int KernelID = 0,
        const int DeviceID = 0
    );
    
    /**
     * This routine returns the number of distinct measurements for key/value
     * pairs in the DB. For a prediction, we need at least three (3)
     * measurements.
     * */
    int getNumberOfDistinctMeasurements() const;

    /**
     * This routine generates the lagrange polynomial.
     * */
    void generateLagrangePolynomial();

    /**
     * This routine generates a runtime prediction for a given N
     * */
    double makeRuntimePrediction(const int N) const;

};

}

#else
typedef struct predict predict;
#endif

#endif
