#ifndef ELAPSD_DEVICE_H
#define ELAPSD_DEVICE_H

#include <string>
#include <vector>
#include <sstream>

//#include "DB/IBDBObject.h"
#include "SQLite.h"

namespace ENHANCE {
//class elapsdDevice : public IBDBObject {
class elapsdDevice : public IElapsdSQLite {
private:
    int id;                         ///< Device ID
	std::string name;				///< Device name
	std::vector<int> subDevices;	///< Vector of subdevices

public:
	/**
	 *
	 * Default constructor. Optionally sets a device name
	 *
	 * @param[in] n The optional device name
	 * */
	elapsdDevice(int _id, std::string n = std::string());

	/**
	 *
	 * Add a new subdevice, identified by it's ID.
	 *
	 * @param[in] s The ID of the subdevice to be added.
	 * */
	void addSubDevice(int s);

	/**
	 *
	 * Print the device name to a stream. If the stream is a file the
	 * output is converted to JSON.
	 *
	 * @param[in,out] out The corresponding stream
	 * @param[in] d A reference to the data
	 * @retval std::ostream The returning stream
	 * */
	friend std::ostream& operator<<(std::ostream &out, const elapsdDevice &d);

    std::string getName() { return name; }

    virtual std::vector<std::string> createSQLInsertObj() const;
};
}

#endif /* ELAPSD_DEVICE_H */
