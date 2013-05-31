#ifndef ELAPSD_DATA_H
#define ELAPSD_DATA_H

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

#include <inttypes.h>

#include "Clock.h"
#include "SQLite.h"

namespace ENHANCE {

class elapsdData : public IElapsdSQLite {

    friend class elapsd;

private:
	elapsdClock timestamp;			        ///< Vector to save the timestamp
	std::vector<elapsdClock> clocks;         ///< Vector to save different timings
	std::map<int, std::string> clockNames;  ///< Map to associate clock IDs with names

	int64_t inBytes;	///< Bytes to be transferred host -> device
	int64_t outBytes;	///< Bytes to be transferred device -> host

	int KernelID;		///< Holds the reference to the used kernel
	int DeviceID;		///< Holds the reference to the used device
    int ThreadID;       ///< Holds the reference to the used thread

	bool timerIsRunning;

public:
	/**
	 * Default constructor inserting the available clocks
	 *
	 * */
	elapsdData();

	virtual ~elapsdData() { }

	inline int getKernelID() const { return KernelID; }
	inline int getDeviceID() const { return DeviceID; }
    inline int getThreadID() const { return ThreadID; }
	inline elapsdClock& getTimeStamp() const { return const_cast<elapsdClock&>(timestamp); }

	inline void startAllTimers() {
		timestamp.takeStartStamp();
		clocks[0].takeStartStamp();
		timerIsRunning = true;
	}

	inline void stopAllTimers() {
	    if (timerIsRunning == false) {
	        throw std::runtime_error("Timer was not started!");
	    }

		clocks[0].takeStopStamp();
		timestamp.takeStopStamp();

		timerIsRunning = false;
	}


    inline friend bool operator<(const elapsdData &x, const elapsdData &y) {
        // Test whether we have different kernels
        if (x.KernelID < y.KernelID) {
            return true;
        } else {
            // Kernels are equal, check more properties
            // Test whether we are on different devices
            if (x.DeviceID < y.DeviceID) {
                return true;
            } else {
                    // Not -> Timestamps are left
                    return x.timestamp < y.timestamp;
            }
        }

        // Error otherwise
        return false;			
    }


	/**
	 * Print the class' content to a stream. If the stream is an ofstream the
	 * output is converted to JSON.
	 *
	 * @param[in,out] out The corresponding stream
	 * @param[in] d A reference to the data
	 * @retval std::ostream The returning stream
	 * */
	friend std::ostream& operator<<(std::ostream &out, const elapsdData &d);

    virtual std::vector<std::string> createSQLInsertObj() const;

};
}

#endif /* ELAPSD_DATA_H */
