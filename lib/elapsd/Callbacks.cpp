#include "../include/elapsd/elapsd.h"
#include "../include/elapsd/celapsd.h"
#include "../include/elapsd/Errors.h"

#include <stdexcept>
#include <iostream>

using namespace ENHANCE;

elapsd* cpp_callback_elapsdInit(
    const char *dbName,
    const char *expName,
    const elapsdParams *p
) {

	elapsd *e;

	try {
        if (p == NULL) {
            e = new elapsd(std::string(dbName), std::string(expName));
        } else {
            e = new elapsd(std::string(dbName), std::string(expName), *p);
        }
	} catch (...) {
		return NULL;
	}

	return e;
}

int cpp_callback_elapsdAddKernel(elapsd *e, int ID, const char *kName) {
	try {
		e->addKernel(ID, std::string(kName));
	} catch (std::runtime_error &e) {
		return E_DUPK;
	}
	return E_OK;
}

int cpp_callback_elapsdAddDevice(elapsd *e, int ID, const char *dName) {
	try {
		e->addDevice(ID, std::string(dName));
	} catch (std::runtime_error &e) {
		return E_DUPD;
	}
	return E_OK;
}

int cpp_callback_elapsdAddSubDeviceToDevice(elapsd *e, const int ID, const int sID) {
	try {
		e->addSubDeviceToDevice(ID, sID);
	} catch (std::runtime_error &e) {
		return E_NOD;
	} catch (std::invalid_argument &e) {
		return E_EQU;
	}
	return E_OK;
}

int cpp_callback_elapsdStartTimer(elapsd *e, int KernelID, int DeviceID) {
	try {
 		e->startTimer(KernelID, DeviceID);
	} catch (std::invalid_argument &e) {
		//Decode exception
		std::string what = e.what();
		if (what.find("Kernel")) {
			return E_NOK;
		} else if (what.find("Device")) {
			return E_NOD;
		} else {
			//Fatal error
			std::cerr << "Fatal error. Exiting.\n";
			std::terminate();
		}
	}
	return E_OK;
}

int cpp_callback_elapsdStopTimer(elapsd *e, int KernelID, int DeviceID) {
	try {
		e->stopTimer(KernelID, DeviceID);
	} catch (std::invalid_argument &e) {
		//Decode exception
		std::string what = e.what();
		if (what.find("Kernel")) {
			return E_NOK;
		} else if (what.find("Device")) {
			return E_NOD;
		} else {
			//Fatal error
			std::cerr << "Fatal error. Exiting.\n";
			std::terminate();
		}
	} catch (std::runtime_error &e) {
		return E_TMR;
	} catch (std::logic_error &e) {
		return E_RES;
	}
	return E_OK;
}

double cpp_callback_elapsdGetLastWallTime(elapsd *e, int KernelID, int DeviceID) {
    return e->getLastWallTime(KernelID, DeviceID);
}

int cpp_callback_elapsdAddKernelDataVolumes(elapsd *e, int KernelID, int DeviceID, int64_t inBytes, int64_t outBytes) {
	try {
		e->addKernelDataVolumes(KernelID, DeviceID, inBytes, outBytes);
	} catch (std::invalid_argument &e) {
		//Decode exception
		std::string what = e.what();
		if (what.find("Kernel")) {
			return E_NOK;
		} else if (what.find("Device")) {
			return E_NOD;
		} else {
			//Fatal error
			std::cerr << "Fatal error. Exiting.\n";
			std::terminate();
		}
	}
	return E_OK;
}

void cpp_callback_elapsdCommitToDB(elapsd *e) {
	e->commitToDB();
}

void cpp_callback_elapsdPrintResults(elapsd *e) {
	std::cout << *e;
}
