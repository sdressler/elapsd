#include "../include/elapsd/elapsd.h"
#include "../include/elapsd/Helpers.h"

namespace ENHANCE {

void elapsd::commitToDB() {

    DMSG("Committing to DB");

    elapsdSQLite db(dbFileName);

    for (tDeviceMap::iterator it = devices.begin(); it != devices.end(); ++it) {

            db.executeInsertQuery(
                (it->second).createSQLInsertObj()
            );

    }

    for (tKernelMap::iterator it = kernels.begin(); it != kernels.end(); ++it) {

            db.executeInsertQuery(
                (it->second).createSQLInsertObj()
            );
    }

    db.beginTransaction();

    for (unsigned int i = 0; i < data.size(); i++) {

        std::list<elapsdData>::const_iterator it;
        for (it = data[i].begin(); it != data[i].end(); ++it) {
            db.executeInsertQuery(it->createSQLInsertObj());
        }

    }

    db.endTransaction();

}

}
