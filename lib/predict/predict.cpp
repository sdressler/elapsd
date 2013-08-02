#include <elapsd/predict/predict.h>

#include <stdexcept>
#include <sstream>

#include <elapsd/sqlite/query.h>

using namespace ENHANCE;

predict::predict(
    const std::string &dbname,
    const elapsdParams &parameters
) {

    /*
     * Ensure, that there is a DB name which is not empty and try to open the DB.
     * */

    if (dbname.empty()) { throw std::runtime_error("No DB provided."); }

    sqlite3 *db = NULL;
    int ret = sqlite3_open_v2(dbname.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);

    if (ret != SQLITE_OK) {
        std::stringstream err;
        err << "Could not open SQLite DB. Reason: " << sqlite3_errmsg(db);
        err << " Path was: " << dbname;
        throw std::runtime_error(err.str());
    }

    /* Create shared ptr */
    db_ = boost::shared_ptr<sqlite3>(db, sqlite3_close);

    /*
     * Ensure, that the parameter set is non-empty and copy its contents.
     * */
    if (parameters.empty()) { throw std::runtime_error("Parameter map empty."); }

    /* Current limitation: only one parameter */
    if (parameters.size() > 1) {
        throw std::runtime_error("Only one parameter is currently supported");
    }

    parameters_.insert(parameters.begin(), parameters.end());

    /* Parameter validation */
    if(!checkMatchingParamsInDB()) {
        throw std::runtime_error("Parameters do not match with DB.");
    }

}

bool predict::checkMatchingParamsInDB() {

    /*
     * For every parameter, check whether it is contained within the response
     * from the DB. If *pcopy* is *not* empty at the very end, we have a
     * mismatch and return "false".
     * */

    std::string qry("\
        SELECT param_name \
        FROM experiment_parameters \
        GROUP BY param_name \
    ");

    boost::shared_ptr<sqlite3_stmt> stmt;
    SQLiteQuery(db_, qry, stmt);

    elapsdParams pcopy(parameters_);

    int ret = sqlite3_step(stmt.get());
    while (ret == SQLITE_ROW) {
    
        const char *pname = (const char*)sqlite3_column_text(stmt.get(), 0);

        if (pcopy.find(pname) != pcopy.end()) { pcopy.erase(pname); }

        ret = sqlite3_step(stmt.get());
    }

    if (ret != SQLITE_DONE) {
        std::stringstream err;
        err << "SQLITE_DONE expected, got " << ret;
        throw std::runtime_error(err.str());
    }

    return pcopy.empty() ? true : false;

}

int predict::getNumberOfDistinctMeasurements() {
    
    /*
     * IMPORTANT NOTE: this routine currently only works under the assumption,
     * that *a single* parameter is used. This is cross-checked in the
     * constructor.
     * */

    std::string qry(
        "SELECT COUNT(DISTINCT param_value) FROM experiment_parameters"
    );

    boost::shared_ptr<sqlite3_stmt> stmt;
    SQLiteQuery(db_, qry, stmt);

    int ret = sqlite3_step(stmt.get());

    if (ret != SQLITE_ROW) {
        std::stringstream err;
        err << "SQLITE_ROW expected, got " << ret;
        throw std::runtime_error(err.str());
    }

    return sqlite3_column_int(stmt.get(), 0);

}
