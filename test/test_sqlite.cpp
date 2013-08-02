#define BOOST_TEST_MODULE sqlite_test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <elapsd/sqlite/query.h>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(query_ptr_test) {

    std::string dbname = "test.db";

    sqlite3 *db_;

    int ret = sqlite3_open_v2(dbname.c_str(), &db_, SQLITE_OPEN_READONLY, NULL);

    if (ret != SQLITE_OK) {
        std::stringstream err;
        err << "Could not open SQLite DB. Reason: " << sqlite3_errmsg(db_);
        err << " Path was: " << dbname;
        throw std::runtime_error(err.str());
    }

    boost::shared_ptr<sqlite3> db(db_, sqlite3_close);

    SQLiteQuery *qry = NULL;
    boost::shared_ptr<sqlite3_stmt> stmt;

    /* Create Query */
    BOOST_REQUIRE_NO_THROW(
        SQLiteQuery(db, "SELECT * FROM experiments", stmt)
    );

    BOOST_CHECK(stmt);

    delete qry;

    BOOST_CHECK(stmt);

    stmt.reset();
    BOOST_CHECK(!stmt);

}
