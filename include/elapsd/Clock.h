#ifndef ELAPSD_CLOCK_H
#define ELAPSD_CLOCK_H

#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>

#ifdef __MACH__
    #include <mach/clock.h>
    #include <mach/mach.h>
#else
    #include <ctime>
#endif

namespace ENHANCE {

class elapsdClock {
private:

    // The following is for Mac OS X
    #ifdef __MACH__
        typedef clock_serv_t clockid_t;
        mach_timespec_t start;
        mach_timespec_t stop;
    #else
        struct timespec start;
	    struct timespec stop;
    #endif

	clockid_t clock;

public:
    #ifdef __MACH__
        elapsdClock();
    #else
	    elapsdClock(const clockid_t &c = CLOCK_REALTIME);
    #endif

	inline void takeStartStamp() {
        #ifdef __MACH__
            clock_get_time(clock, &start);
        #else
            clock_gettime(clock, &start);
        #endif
    }

	inline void takeStopStamp() {
        #ifdef __MACH__
            clock_get_time(clock, &stop);
        #else
            clock_gettime(clock, &stop);
        #endif
    }

	friend bool operator<(const elapsdClock &x, const elapsdClock &y);

	friend std::ostream& operator<<(std::ostream &out, const elapsdClock &c);
	
	double getTimeDifference() const;

	std::pair<uint64_t, uint64_t> getIntegerPair() const;
    std::vector<double> getDoublePairs(struct timespec &base) const;

//	virtual tByteVectorMap convertToByteVectorMap() const;
};
}

#endif /* ELAPSD_CLOCK_H */
