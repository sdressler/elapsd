#include <elapsd/predict/predict.h>

#include <stdexcept>
#include <sstream>

#include <elapsd/sqlite/query.h>

using namespace ENHANCE;

predict::predict(
    const std::string &dbname,
    const std::vector<std::string> &parameters,
    int KernelID, int DeviceID
) {

    /*
     * Ensure, that there is a DB name which is not empty and try to open the DB.
     * */

    if (dbname.empty()) { throw std::invalid_argument("No DB provided."); }

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
    if (parameters.empty()) {
        throw std::invalid_argument("No parameters given.");
    }

    parameters_ = parameters;

    KernelID_ = KernelID;
    DeviceID_ = DeviceID;

}

std::vector<predict::MultDimPointT> predict::getDataPointsFromDB() const {

    std::vector<predict::MultDimPointT> points;

    std::stringstream qry;

    qry << "SELECT param_name, param_value, (ts_stop - ts_start) * 1.0e-9 as y "
        << "FROM data "
        << "NATURAL JOIN experiment_parameters "
        << "WHERE id_kernel = " << KernelID_ << " AND "
        <<       "id_device = " << DeviceID_ << " "
        << "ORDER BY y, param_name";

    boost::shared_ptr<sqlite3_stmt> stmt;
    SQLiteQuery(db_, qry.str(), stmt);

    int ret = sqlite3_step(stmt.get());
    while (ret == SQLITE_ROW) {

        MultDimPointT point;

        std::vector<std::string>::const_iterator it = parameters_.begin();
        for (; it != parameters_.end(); ++it) {

            if (ret != SQLITE_ROW) {
                throw std::runtime_error("SQLITE_ROW was expected.");
            }
            
            const char *p_name  = (const char*)sqlite3_column_text(stmt.get(), 0);

            /* p_name must match */
            if (std::string(p_name) != *it) {
                throw std::runtime_error("Parameter names do not match.");
            }

            int p_value = sqlite3_column_int(stmt.get(), 1);

            predict::y_type y = static_cast<predict::y_type>(
                sqlite3_column_double(stmt.get(), 2)
            );
            
            point.first.push_back(p_value);
            point.second = y; // y are always equal

            ret = sqlite3_step(stmt.get());

        }

        points.push_back(point);

    }

    return points;

}

bool predict::checkMatchingParamsInDB() const {

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
/*
 *int predict::getNumberOfDistinctMeasurements() const {
 */
    
    /*
     * IMPORTANT NOTE: this routine currently only works under the assumption,
     * that *a single* parameter is used. This is cross-checked in the
     * constructor.
     * */

/*
 *    std::stringstream ss;
 *
 *    ss << "SELECT COUNT(DISTINCT param_value) "
 *       << "FROM experiment_parameters "
 *       << "NATURAL JOIN data "
 *       << "WHERE id_kernel = " << KernelID_ << " AND id_device = " << DeviceID_;
 *
 *    boost::shared_ptr<sqlite3_stmt> stmt;
 *    SQLiteQuery(db_, ss.str(), stmt);
 *
 *    int ret = sqlite3_step(stmt.get());
 *
 *    if (ret != SQLITE_ROW) {
 *        std::stringstream err;
 *        err << "SQLITE_ROW expected, got " << ret;
 *        throw std::runtime_error(err.str());
 *    }
 *
 *    return sqlite3_column_int(stmt.get(), 0);
 *
 *}
 */
/*
 *
 *void predict::generateLagrangePolynomial() {
 *
 *    if (getNumberOfDistinctMeasurements() < 3) {
 *        throw std::runtime_error("Not enough measurements!");
 *    }
 *
 *    typedef Lagrange<3, int, double>::Point Point;
 *
 *    [> Load all available X/Y Points <]
 *    std::stringstream ss;
 *
 *    ss << "SELECT param_value, avg((ts_stop - ts_start) * 1.0e-9) "
 *       << "FROM data "
 *       << "NATURAL JOIN experiment_parameters "
 *       << "WHERE id_kernel = " << KernelID_ << " "
 *       <<   "AND id_device = " << DeviceID_ << " "
 *       << "GROUP BY param_value";
 *
 *    DMSG(ss.str());
 *
 *    boost::shared_ptr<sqlite3_stmt> stmt;
 *    SQLiteQuery(db_, ss.str(), stmt);
 *
 *    std::vector<Point> points;
 *
 *    int ret = sqlite3_step(stmt.get());
 *    while (ret == SQLITE_ROW) {
 *    
 *        int X = sqlite3_column_int(stmt.get(), 0);
 *        double Y = sqlite3_column_double(stmt.get(), 1);
 *
 *        points.push_back(std::make_pair(X, Y));
 *
 *        ret = sqlite3_step(stmt.get());
 *    }
 *
 *    if (ret != SQLITE_DONE) {
 *        std::stringstream err;
 *        err << "SQLITE_DONE expected, got " << ret;
 *        throw std::runtime_error(err.str());
 *    }
 *
 *    L = Lagrange<3, int, double>(points);
 *    L.findBestPolynomial();
 *
 *}
 *
 */

