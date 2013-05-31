#ifndef ELAPSD_CONTAINER_H
#define ELAPSD_CONTAINER_H

#include "Kernel.h"
#include "Device.h"
#include "Data.h"

#include <string>
#include <set>
#include <map>
#include <sstream>

namespace ENHANCE {
	
typedef std::map<int, elapsdKernel> tKernelMap;
typedef std::map<int, elapsdDevice> tDeviceMap;

class elapsdContainer {

protected:
	tKernelMap kernels;	///< Holds the kernels with ID and name
	tDeviceMap devices;	///< Holds the devices with ID and name

	tDeviceMap& getDevices() { return devices; }

};
}

#endif /* ELAPSD_CONTAINER_H */
