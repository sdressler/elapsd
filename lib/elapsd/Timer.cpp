/*
 * elapsdTimer.cpp
 *
 *  Created on: Apr 19, 2013
 *      Author: bzcdress
 */

#include <omp.h>
#include "../include/elapsd/elapsd.h"
#include "../include/elapsd/Helpers.h"

namespace ENHANCE {

//Start the time measurement of a specific kernel
void elapsd::startTimer(const int KernelID, const int DeviceID) {

    t_elapsd_id elapsd_id = getElapsdID(
        (uint32_t)(getThreadID() - getThreadGroupID()),
        (uint16_t)KernelID,
        (uint16_t)DeviceID
    );

    //Retrieve a relative ID
    #pragma omp critical
    {
        if (thr_map.find(elapsd_id) == thr_map.end()) {
            thr_map[elapsd_id] = id_relative;
            id_relative++;
        }
    }

    uint64_t id = thr_map[elapsd_id];

    if (id > max_units) { throw std::runtime_error("Thread overflow."); }

    DMSG("TMR START K: " << getElapsdKernelID(elapsd_id) <<
                  " D: " << getElapsdDeviceID(elapsd_id) <<
                  " T: " << getElapsdThreadID(elapsd_id) <<
                  " EID: " << std::hex << elapsd_id << std::dec <<
                  " ID: " << id);

    data[id].push_back(elapsdData());
    data[id].back().startAllTimers();

};

//Stop the time measurement and save the measured time
void elapsd::stopTimer(const int KernelID, const int DeviceID) {
    
    t_elapsd_id elapsd_id = getElapsdID(
        (uint32_t)(getThreadID() - getThreadGroupID()),
        (uint16_t)KernelID,
        (uint16_t)DeviceID
    );

    uint64_t id = thr_map[elapsd_id];
    
    DMSG("TMR STOP  K: " << getElapsdKernelID(elapsd_id) <<
                  " D: " << getElapsdDeviceID(elapsd_id) <<
                  " T: " << getElapsdThreadID(elapsd_id) <<
                  " EID: " << std::hex << elapsd_id << std::dec <<
                  " ID: " << id);

    elapsdData *edata = &(data[id].back());

    edata->stopAllTimers();
    edata->KernelID = getElapsdKernelID(elapsd_id);
    edata->DeviceID = getElapsdDeviceID(elapsd_id);
    edata->ThreadID = getElapsdThreadID(elapsd_id);

}

// Take a TimeSpec structure and convert it to double, seconds
double elapsd::convTimeSpecToDoubleSeconds(const struct timespec &t) const {
    double tt = (double)t.tv_nsec * 1.0e-9;
    tt += (double)t.tv_sec;

    return tt;
}
    
double elapsd::getLastWallTime(const int KernelID, const int DeviceID) const {
    
    t_elapsd_id elapsd_id = getElapsdID(
        (uint32_t)(getThreadID() - getThreadGroupID()),
        (uint16_t)KernelID,
        (uint16_t)DeviceID
    );

    thr_map_type::const_iterator it = thr_map.find(elapsd_id);

    if (it == thr_map.end()) {
        throw std::runtime_error("Could not find ID in thr_map.");
    }

    uint64_t id = it->second;

    return (&(data[id].back()))->timestamp.getTimeDifference();

}

}
