#include "../include/elapsd/Clock.h"

#include <cstring>
#include <sstream>

namespace ENHANCE {

#ifdef __MACH__
elapsdClock::elapsdClock() {
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock);
}
#else
elapsdClock::elapsdClock(const clockid_t &c) {
	clock = c;
}
#endif

bool operator<(const elapsdClock &x, const elapsdClock &y) {
	if (x.start.tv_sec < y.start.tv_sec || x.start.tv_nsec < y.start.tv_nsec) {
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream &out, const elapsdClock &c) {
	out << " Start: " << c.start.tv_sec << " s " << c.start.tv_nsec << " ns";
	out << " / Stop: " << c.stop.tv_sec  << " s " << c.stop.tv_nsec  << " ns\n";

	return out;
}

double elapsdClock::getTimeDifference() const {

	struct timespec temp;
	if ((stop.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = stop.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+stop.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = stop.tv_sec-start.tv_sec;
		temp.tv_nsec = stop.tv_nsec-start.tv_nsec;
	}

	//Convert the difference to double
	double diff = (double)temp.tv_sec * 1.0e9 + (double)temp.tv_nsec;

	return diff / 1.0e9;
}

std::pair<uint64_t, uint64_t> elapsdClock::getIntegerPair() const {

    uint64_t start_ns, stop_ns;

    start_ns = (uint64_t)(start.tv_sec) * 1000000000 + (uint64_t)(start.tv_nsec);
    stop_ns  = (uint64_t)(stop.tv_sec)  * 1000000000 + (uint64_t)(stop.tv_nsec);

    return std::make_pair(start_ns, stop_ns);

}

std::vector<double> elapsdClock::getDoublePairs(struct timespec &base) const {

    std::vector<double> x;

    x.push_back((double)(start.tv_sec - base.tv_sec) * 1.0e9 + (double)(start.tv_nsec - base.tv_nsec));
    x.push_back((double)(stop.tv_sec - base.tv_sec) + (double)(stop.tv_nsec - base.tv_nsec));

    return x;

}

}
