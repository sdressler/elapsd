#include <elapsd/predict/predict.h>

#include <stdexcept>
#include <sstream>

using namespace ENHANCE;

predict::predict(const std::string &dbname) {

    /*
     * Ensure, that there is a DB name which is not empty and try to open the DB.
     * */

    if (dbname.empty()) { throw std::runtime_error("No DB provided."); }

    int ret = sqlite3_open_v2(dbname.c_str(), &db_, SQLITE_OPEN_READWRITE, NULL);

    if (ret != SQLITE_OK) {
        std::stringstream err;
        err << "Could not open SQLite DB. Reason: " << sqlite3_errmsg(db_);
        err << " Path was: " << dbname;
        throw std::runtime_error(err.str());
    }

}

predict::~predict() {
    sqlite3_close(db_);
}
