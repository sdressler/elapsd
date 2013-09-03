#ifndef SQLITE_QUERY_H_INC_
#define SQLITE_QUERY_H_INC_

#include <sqlite3.h>
#include <string>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

struct SQLiteQuery {
    
    SQLiteQuery(
        const boost::shared_ptr<sqlite3> &db,
        const std::string &qry,
        boost::shared_ptr<sqlite3_stmt> &stmt
    ) {
    
        sqlite3_stmt *stmt_;
        int ret = sqlite3_prepare_v2(db.get(), qry.c_str(), -1, &stmt_, NULL);

        if (ret != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errmsg(db.get())));
        }

        stmt = boost::shared_ptr<sqlite3_stmt>(stmt_, sqlite3_finalize);

    }

};

#endif /* SQLITE_QUERY_H_INC_ */
