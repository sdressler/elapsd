#include <elapsd/predict/predict.h>

#include <stdexcept>
#include <sstream>

#include <elapsd/sqlite/query.h>

using namespace ENHANCE;

predict::predict(
    const std::string &dbname,
    const std::vector<std::string> &parameters,
    const int KernelID,
    const int DeviceID
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
    if (parameters.empty()) { throw std::runtime_error("No parameters given."); }

    /* Current limitation: only one parameter */
    if (parameters.size() > 1) {
        throw std::runtime_error("Currently, only one parameter is supported");
    }

    parameters_ = parameters;

    /* Parameter validation */
    if(!checkMatchingParamsInDB()) {
        throw std::runtime_error("Parameters do not match with DB.");
    }

    KernelID_ = KernelID;
    DeviceID_ = DeviceID;

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

    std::vector<std::string> pcopy(parameters_);

    int ret = sqlite3_step(stmt.get());
    while (ret == SQLITE_ROW) {
    
        const char *pname = (const char*)sqlite3_column_text(stmt.get(), 0);

        std::vector<std::string>::iterator it = std::find(
            pcopy.begin(), pcopy.end(), std::string(pname)      
        );

        if (it != pcopy.end()) { pcopy.erase(it); }

        ret = sqlite3_step(stmt.get());
    }

    if (ret != SQLITE_DONE) {
        std::stringstream err;
        err << "SQLITE_DONE expected, got " << ret;
        throw std::runtime_error(err.str());
    }

    return pcopy.empty() ? true : false;

}

int predict::getNumberOfDistinctMeasurements() const {
    
    /*
     * IMPORTANT NOTE: this routine currently only works under the assumption,
     * that *a single* parameter is used. This is cross-checked in the
     * constructor.
     * */

    std::stringstream ss;

    ss << "SELECT COUNT(DISTINCT param_value) "
       << "FROM experiment_parameters "
       << "NATURAL JOIN data "
       << "WHERE id_kernel = " << KernelID_ << " AND id_device = " << DeviceID_;

    boost::shared_ptr<sqlite3_stmt> stmt;
    SQLiteQuery(db_, ss.str(), stmt);

    int ret = sqlite3_step(stmt.get());

    if (ret != SQLITE_ROW) {
        std::stringstream err;
        err << "SQLITE_ROW expected, got " << ret;
        throw std::runtime_error(err.str());
    }

    return sqlite3_column_int(stmt.get(), 0);

}

void predict::generateLagrangePolynomial() {

    if (getNumberOfDistinctMeasurements() < 3) {
        throw std::runtime_error("Not enough measurements!");
    }

    typedef Lagrange<3, int, double>::Point Point;

    /* Load all available X/Y Points */
    std::string qry(
        "SELECT param_value, avg((ts_stop - ts_start) * 1.0e-9) \
         FROM data \
         NATURAL JOIN experiment_parameters \
         GROUP BY param_value;"
    );

    boost::shared_ptr<sqlite3_stmt> stmt;
    SQLiteQuery(db_, qry, stmt);

    std::vector<Point> points;

    int ret = sqlite3_step(stmt.get());
    while (ret == SQLITE_ROW) {
    
        int X = sqlite3_column_int(stmt.get(), 0);
        double Y = sqlite3_column_double(stmt.get(), 1);

        points.push_back(std::make_pair(X, Y));

        ret = sqlite3_step(stmt.get());
    }

    if (ret != SQLITE_DONE) {
        std::stringstream err;
        err << "SQLITE_DONE expected, got " << ret;
        throw std::runtime_error(err.str());
    }

    L = Lagrange<3, int, double>(points);
    L.findBestPolynomial();

}

double predict::makeRuntimePrediction(const int N) const {

    double runtime = L.getLagrangeInterpolation(N);

    DMSG("Predicted runtime: " << runtime << " s");

    return runtime;
}

