#ifndef PREDICT_H_INC_
#define PREDICT_H_INC_

#ifdef __cplusplus

#include <elapsd/sqlite/query.h>
#include <elapsd/Params.h>
#include <elapsd/predict/IPredictionModel.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <sqlite3.h>

#include <elapsd/Errors.h>

/**
 * This is the main predictor class
 * */

namespace ENHANCE {

class predict {

public:
    typedef std::pair<std::vector<int>, float> MultDimPointT;
    typedef MultDimPointT::first_type  x_type;
    typedef MultDimPointT::second_type y_type;

private:
    boost::shared_ptr<sqlite3> db_;
    boost::shared_ptr<IPredictionModel<MultDimPointT> > predictionModel_;

    std::vector<std::string> parameters_;

    int KernelID_;
    int DeviceID_;

    /**
     * This routine checks, whether the provided parameters match those from the
     * DB. That is, their names are matched against the *experiment_parameters*
     * table of the DB. Exceptions are thrown on mismatches.
     * */
    bool checkMatchingParamsInDB() const;

    /**
     * This routine retrieves all necessary parameter points from the DB. That
     * is, it selects all X/Y values matching the given parameters.
     * */
    std::vector<MultDimPointT> getDataPointsFromDB() const;

public:
    /**
     * This replaces the default constructor, because we really need a DB which
     * is opened during object construction
     * */
    predict(
        const std::string &dbname,
        const std::vector<std::string> &parameters,
        int KernelID = 0, int DeviceID = 0
    );

    template<class T>
    void createPredictionModel() {

        /**
         * First, ensure, that the provided parameters match.
         * */
        if (!checkMatchingParamsInDB()) {
            throw param_mismatch_error("Parameters do not match DB.");
        }

        /* Create the prediction model */
        predictionModel_ = boost::shared_ptr<T>(new T());

        /**
         * Retrieve all points from the DB and provide the to the prediction model.
         * */
        predictionModel_->updateDataPoints(getDataPointsFromDB());

    }

    /* This simply delegates the runtime prediction call to the model */
    y_type getRuntimePrediction(const x_type &x) {
        return predictionModel_->getRuntimePrediction(x);
    }

    y_type getRuntimePrediction(const x_type::value_type &x) {
        return predictionModel_->getRuntimePrediction(x_type(1,x));
    }
    
};

}

#else
typedef struct predict predict;
#endif

#endif
