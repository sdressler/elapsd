#ifndef ELAPSD_H
#define ELAPSD_H

// C header
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

// This only checks whether we are on a POSIX system or not
#ifndef _POSIX_TIMERS
	#error _POSIX_TIMERS not available
#endif
#ifndef _POSIX_THREAD_CPUTIME
	#error _POSIX_THREAD_CPUTIME not available
#endif
#ifndef _POSIX_MONOTONIC_CLOCK
	#error _POSIX_MONOTONIC_CLOCK not available
#endif

#ifdef __cplusplus

#include <map>
#include <cmath>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <sys/syscall.h>

#include "Container.h"

namespace ENHANCE {

    typedef std::vector<std::list<elapsdData> > tDataVector;

/**
 * This class contains the public interface to elaps'd. Internally, it acts as a
 * wrapper for all provided methods.
 *
 * \author    Sebastian Dreßler (ZIB)
 * \date      2012-05-24
 * \version   0.1
 * \copyright BSD License
 *
 * */
class elapsd : public elapsdContainer {
private:
    tDataVector data;
    std::vector<unsigned int> dataSizeVector;

    inline pid_t getThreadID() { return syscall(SYS_gettid); }
    inline pid_t getThreadGroupID() { return syscall(SYS_getpid); }

    std::string dbFileName;
    
    static uint64_t    experiment_id;
    static std::string experiment_name;
    static long int    experiment_date;

    typedef uint64_t t_elapsd_id;
    inline t_elapsd_id getElapsdID(uint32_t t, uint16_t k, uint16_t d) {
        return     (uint64_t)t
               + (((uint64_t)k) << 32)
               + (((uint64_t)d) << 48);
    }

    inline uint32_t getElapsdThreadID(t_elapsd_id elapsd_id) {
        return (uint32_t)(elapsd_id);
    }

    inline uint16_t getElapsdKernelID(t_elapsd_id elapsd_id) {
        return (uint16_t)(elapsd_id >> 32);
    }
    
    inline uint16_t getElapsdDeviceID(t_elapsd_id elapsd_id) {
        return (uint16_t)(elapsd_id >> 48);
    }

	unsigned long max_units;
	std::map<t_elapsd_id, uint64_t> thr_map;
	uint64_t id_relative;

	/**
	 * Captures the current time and converts it to double
	 *
	 * @param[in] t Pointer to the timespec variable for saving the result
	 * @return The time seconds converted to double
	 * */
	double convTimeSpecToDoubleSeconds(const struct timespec &t);

	/**
	 * Checks whether a specific device ID exists
	 *
	 * @exception std::invalid_argument If the device does not exist
	 * @param[in] ID The ID of the device
	 * */
    inline void checkDeviceExistance(const int ID) {
        if (devices.find(ID) == devices.end()) {
            throw std::invalid_argument("Device ID not valid.");
        }
    }
	
	/**
	 * Checks whether a specific kernel ID exists
	 *
	 * @exception std::invalid_argument If the kernel does not exist
	 * @param[in] ID The ID of the kernel
	 * */
    inline void checkKernelExistance(const int ID) {
        if (kernels.find(ID) == kernels.end()) {
            throw std::invalid_argument("Kernel ID not valid.");
        }
    }

    void resizeTemporaryDataObject();

public:
    static struct timespec experiment_starttime;
	static uint64_t& getExperimentID() { return experiment_id; }

    /**
     *
     * elaps'd constructor with parameters for the name of the DB file and the
     * current experiment identifier. 
     *
     * @param[in] _dbFileName Where to store the DB
     * @param[in] _expName    The name of the experiment
     * */
	elapsd(const std::string &_dbFileName, const std::string &_expName);
	~elapsd();

	/**
	 *
	 * Commit the collected data to the DB. If the DB does not exist yet, it
     * will be created. The name of the DB is set when initializing elaps'd.
	 * 
	 * */
    void finalize();
	void commitToDB();

	/**
	 * Adds a new kernel to be measured.
	 *
	 * @exception std::runtime_error If a kernel with the given ID already exists
	 * @param[in] ID The unique ID of the kernel
	 * @param[in] kName The (optional) name of the kernel
	 * */
	tKernelMap::iterator addKernel(const int ID, const std::string &kName = std::string());

	/**
	 * Adds a new device where kernels run on.
	 *
	 * @exception std::runtime_error If a device with the given ID already exists
	 * @param[in] ID The unique ID of the device
	 * @param[in] dName The (optional) name of the device
	 * */
	tDeviceMap::iterator addDevice(const int ID, const std::string &dName = std::string());

	/**
	 * Adds a new subdevice to an existing device.
	 *
	 * @exception std::runtime_error If the subdevice to be added or the device
	 * where the subdevice shall be added does not exist
	 * @exception std::invalid_argument If both ID's are the same.
	 *
	 * @param[in] ID The unique ID of the device where the subdevice shall be added
	 * @param[in] sID The unique ID of the subdevice to be added to the device
	 * */
	void addSubDeviceToDevice(const int ID, const int sID);

	/**
	 * Starts the time measurement for a specific kernel / device combination
	 *
	 * @exception std::invalid_argument If the kernel ID or the device ID is invalid (what() provides details)
	 * @param[in] KernelID The ID of the kernel
	 * @param[in] DeviceID The ID of the device
	 * */
	void startTimer(const int KernelID, const int DeviceID);

	/**
	 * Stops the time measurement for a specific kernel / device combination
	 *
	 * @exception std::invalid_argument If the kernel ID or the device ID is invalid (what() provides details)
	 * @exception std::runtime_error If the starttime was not recorded
	 * @exception std::logic_error If the time difference is negative
	 * @param[in] KernelID The ID of the kernel
	 * @param[in] DeviceID The ID of the device
	 * */
	void stopTimer(const int KernelID, const int DeviceID);

	/**
	 * Registers the data to be transferred for a specific kernel / device combination
	 *
     * @warning This feature currently is an experimental one.
	 * @exception std::invalid_argument If the kernel ID or the device ID is invalid (what() provides details)
	 * @param[in] KernelID The ID of the kernel
	 * @param[in] DeviceID The ID of the device
	 * @param[in] inBytes  The number of Bytes to be transferred from <b>host to device</b>
	 * @param[in] outBytes The number of Bytes to be transferred from <b>device to host</b>
	 * */
	void addKernelDataVolumes(int KernelID,	int DeviceID, int64_t inBytes, int64_t outBytes);

	/**
	 * Prints the content of the object to given std::ostream.
	 *
	 * @param[in,out] out Stream for output
	 * @param[in] e The object to be printed
	 * @retval The output stream
	 * */
	friend std::ostream& operator<<(std::ostream &out, const elapsd &e);

};
}

#else
typedef struct elapsd elapsd;
#endif

#endif /* ELAPSD_H */
