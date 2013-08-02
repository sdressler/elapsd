#ifndef PREDICT_H_INC_
#define PREDICT_H_INC_

#include <elapsd/Params.h>

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
    elapsdParams parameters_;

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
    explicit predict(
        const std::string &dbname,
        const elapsdParams &parameters
    );
    
    /**
     * This routine returns the number of distinct measurements for key/value
     * pairs in the DB. For a prediction, we need at least three (3)
     * measurements.
     * */
    int getNumberOfDistinctMeasurements();

};

}

#endif
