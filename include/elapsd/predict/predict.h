#ifndef PREDICT_H_INC_
#define PREDICT_H_INC_

#include <string>
#include <sqlite3.h>

/**
 * This is the main predictor class
 * */

namespace ENHANCE {

class predict {
    
private:
    sqlite3 *db_;

public:
    /**
     * This replaces the default constructor, because we really need a DB which
     * is opened during object construction
     * */
    predict(const std::string &dbname);

    /**
     * Custom destructor which will close the DB once the object is destroyed.
     * */
    ~predict();

};

}

#endif
