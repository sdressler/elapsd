#include "../include/elapsd/Data.h"
#include "../include/elapsd/elapsd.h"

#include <cstring>

namespace ENHANCE {
elapsdData::elapsdData() {
	KernelID = -1;
	DeviceID = -1;
	ThreadID = 0;
	inBytes = 0;
	outBytes = 0;
	timerIsRunning = false;

    #ifdef __MACH__
        clocks.push_back(elapsdClock());
    #else
	    clocks.push_back(elapsdClock(CLOCK_PROCESS_CPUTIME_ID));
    #endif
    clockNames[0] = "CPU clock";
    clockNames[1] = "Wall clock";
}

std::ostream& operator<<(std::ostream &out, const elapsdData &d) {

    out << "K: " << d.KernelID << ", "
        << "D: " << d.DeviceID << "\n";
	out << std::scientific << std::setprecision(9);
	out << "Wall Clock:\t" << d.timestamp.getTimeDifference() << " s\n";
    out << "CPU  Clock:\t" << d.clocks[0].getTimeDifference() << " s\n";

	return out;
}

std::vector<std::string> elapsdData::createSQLInsertObj() const {

    std::vector<std::string> x;
    std::stringstream q;

    q   << "INSERT INTO data "
        << "("
        <<      "id_kernel, id_device, "
        <<      "ts_start, "
        <<      "ts_stop, "
        <<      "cpuclock_start, "
        <<      "cpuclock_stop, "
        <<      "tid, data_in, data_out,"
        <<      "id_experiment"
        << ") "
        << "VALUES ("
        <<      KernelID << ", "
        <<      DeviceID << ", ";

    std::pair<uint64_t, uint64_t> start_stop_pair = timestamp.getIntegerPair();
    q << start_stop_pair.first << ", " << start_stop_pair.second << ", ";

    std::vector<elapsdClock>::const_iterator it;
    for (it = clocks.begin(); it != clocks.end(); ++it) {
        start_stop_pair = it->getIntegerPair();
        q << start_stop_pair.first << ", " << start_stop_pair.second << ", ";
    }

      q <<      ThreadID << ", "
        <<      inBytes << ", "
        <<      outBytes << ", "
        <<      "'" << elapsd::getExperimentID() << "'"
        << ")";

    x.push_back(q.str());

    return x;

}

}

