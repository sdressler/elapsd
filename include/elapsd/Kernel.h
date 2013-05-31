#ifndef ELAPSD_KERNEL_H
#define ELAPSD_KERNEL_H

#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include "SQLite.h"

namespace ENHANCE {

class elapsdKernel : public IElapsdSQLite {
private:
    int id;           ///< Holds the ID of the kernel
	std::string name; ///< Holds the kernel name

public:
	/**
	 * Create a new kernel with a name for easier identification
	 *
     * @param[in] id The id of the new kernel
	 * @param[in] s The name of the new kernel
	 * */
	elapsdKernel(int id, std::string s);

    friend std::ostream& operator<<(std::ostream &out, const elapsdKernel &k);

    virtual std::vector<std::string> createSQLInsertObj() const;
};
}

#endif /* ELAPSD_KERNEL_H */
