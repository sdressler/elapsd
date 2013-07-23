#include "../include/elapsd/celapsd.h"
#include "../include/elapsd/elapsd.h"

elapsd* elapsdInit(
    const char *dbName,
    const char *expName,
    const elapsdParams *p
) {	return cpp_callback_elapsdInit(dbName, expName, p); }

int elapsdAddKernel(elapsd *e, int ID, const char *kName) {
	return cpp_callback_elapsdAddKernel(e, ID, kName);
}

int elapsdAddDevice(elapsd *e, int ID, const char *dName) {
	return cpp_callback_elapsdAddDevice(e, ID, dName);
}

int elapsdAddSubDeviceToDevice(elapsd *e, const int ID, const int sID) {
	return cpp_callback_elapsdAddSubDeviceToDevice(e, ID, sID);
}

int elapsdStartTimer(elapsd *e, int KernelID, int DeviceID) {
	return cpp_callback_elapsdStartTimer(e, KernelID, DeviceID);
}

int elapsdStopTimer(elapsd *e, int KernelID, int DeviceID) {
	return cpp_callback_elapsdStopTimer(e, KernelID, DeviceID);
}

int elapsdAddKernelDataVolumes(elapsd *e, int KernelID, int DeviceID,
                                          int64_t inBytes, int64_t outBytes) {

	return cpp_callback_elapsdAddKernelDataVolumes(e, KernelID, DeviceID,
                                                      inBytes, outBytes);

}

void elapsdCommitToDB(elapsd *e) {
	cpp_callback_elapsdCommitToDB(e);
}

void elapsdPrintResults(elapsd *e) {
    cpp_callback_elapsdPrintResults(e);
}
