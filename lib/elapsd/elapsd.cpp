/**
 * Implementation of elapsd C++ Interface
 *
 * @author  Sebastian Dressler (ZIB)
 * @date    2012-05-24
 *
 * */

#include <fstream>
#include <sstream>
#include <iostream>

#include <omp.h>

#include <ctime>

#include "../include/elapsd/elapsd.h"
#include "../include/elapsd/SQLite.h"
#include "../include/elapsd/celapsd.h"
#include "../include/elapsd/Helpers.h"

namespace ENHANCE {

uint64_t        elapsd::experiment_id        = 0;
std::string     elapsd::experiment_name      = "";
long int        elapsd::experiment_date      = 0;
struct timespec elapsd::experiment_starttime = {0,0};

elapsd::elapsd(
    const std::string &_dbFileName,
    const std::string &expName,
    const elapsdParams &_expParams
) {

    if (_dbFileName == std::string("")) {
        dbFileName = std::string("elapsd.db");
    } else {
        dbFileName = _dbFileName;
    }

    DMSG("DB: " << _dbFileName);
    DMSG("ExpName: " << expName);

    max_units = MAX_UNITS; // omp_get_num_procs();
    id_relative = 0;

	DMSG("Max Units: " << max_units);

	elapsdSQLite db(dbFileName);

	experiment_id   = db.getLastExperimentID() + 1;
    experiment_name = expName;
    experiment_date = static_cast<long int>(std::time(NULL));

    clock_gettime(CLOCK_REALTIME, &experiment_starttime);

    DMSG("ExpID: " << experiment_id);

    // Write the experiment
    std::stringstream q;

    q << "INSERT OR IGNORE INTO experiments ( \
            id, date, name, start_s, start_ns \
          ) VALUES("
      << "'" << experiment_id   << "', "
             << experiment_date << ", "
      << "'" << experiment_name << "', "
             << experiment_starttime.tv_sec << ", "
             << experiment_starttime.tv_nsec
      << ")";

    db.executeInsertQuery(q.str());

    if (_expParams.empty() == false) {
    
        // Cleanup
        q.str("");

        // Write experiment parameters
        q << "INSERT OR IGNORE INTO experiment_parameters ( \
                  id_experiment, param_name, param_value \
              ) VALUES ";

        std::map<std::string, int>::const_iterator it;
        for (it = _expParams.begin(); it != _expParams.end(); ++it) {
            q << "("
                << experiment_id << ", "
                << "'" << it->first << "', "
                << it->second
              << "), ";
        }

        // Remove last ", " and execute
        std::string q_str = q.str();
        q_str.erase(q_str.end() - 2, q_str.end());

        db.executeInsertQuery(q_str);

    }

    DMSG("Initialized");

}

elapsd::~elapsd() { DMSG("elapsd Destructor."); }

void elapsd::finalize() { commitToDB(); }

void elapsd::resizeTemporaryDataObject() {

    size_t num_threads = max_units; //omp_get_num_procs();

	size_t k_size = kernels.size();
	size_t d_size = devices.size();

	size_t requestedSize = (k_size + d_size) * num_threads;

	data.clear();
    data.resize(requestedSize, std::list<elapsdData>());

}

// Add a new kernel with unique ID and optional kernel name
tKernelMap::iterator elapsd::addKernel(const int ID, const std::string &kName) {
    // Try to insert, if it already exists -> exception
    std::pair<tKernelMap::iterator, bool> r = kernels.insert(
        std::pair<int, elapsdKernel>(ID, elapsdKernel(ID, kName))
    );

    // Check for overwrite attempt
    if (r.second == false) {
        throw std::runtime_error("Kernel already exists.");
    }

    resizeTemporaryDataObject();

    return r.first;
}

// Add a new device with unique ID and optional device name
tDeviceMap::iterator elapsd::addDevice(const int ID, const std::string &dName) {
    // Try to insert
    std::pair<tDeviceMap::iterator, bool> r = devices.insert(
        std::pair<int, elapsdDevice>(ID, elapsdDevice(ID, dName))
    );

    // Check for overwrite attempt
    if (r.second == false) {
        throw std::runtime_error("Device already exists.");
    }

    resizeTemporaryDataObject();

    return r.first;
}

// Add a subdevice to another device
void elapsd::addSubDeviceToDevice(const int ID, const int sID) {
    // Check if both ID's are the same
    if (ID == sID) {
        throw std::invalid_argument("ID == sID!");
    };

    // Check if both devices exist
    checkDeviceExistance(ID);
    checkDeviceExistance(sID);

    // Add sID to ID
    tDeviceMap::iterator it = devices.find(ID);
    (it->second).addSubDevice(sID);
}

// Take a TimeSpec structure and convert it to double, seconds
double elapsd::convTimeSpecToDoubleSeconds(const struct timespec &t) {
    double tt = (double)t.tv_nsec * 1.0e-9;
    tt += (double)t.tv_sec;

    return tt;
}

void elapsd::addKernelDataVolumes(
    int KernelID, int DeviceID, int64_t inBytes, int64_t outBytes
) {
    
    t_elapsd_id elapsd_id = getElapsdID(
        (uint32_t)(getThreadID() - getThreadGroupID()),
        (uint16_t)KernelID,
        (uint16_t)DeviceID
    );

    uint64_t id = thr_map[elapsd_id];
    
    DMSG("TMR KDV   K: " << getElapsdKernelID(elapsd_id) <<
                  " D: " << getElapsdDeviceID(elapsd_id) <<
                  " T: " << getElapsdThreadID(elapsd_id) <<
                  " EID: " << std::hex << elapsd_id << std::dec <<
                  " ID: " << id);

    data[id].back().inBytes = inBytes;
    data[id].back().outBytes = outBytes;

}

std::ostream& operator<<(std::ostream &out, const elapsd &e) {

    std::map<int, elapsdDevice>::const_iterator dit;
    out << "Devices:\n";
    for (dit = e.devices.begin(); dit != e.devices.end(); ++dit) {
        out << "\tID: " << dit->first << " Name: " << dit->second << "\n";
    }
    out << "\n";

    tKernelMap::const_iterator kit; 
    out << "Kernels:\n";
    for (kit = e.kernels.begin(); kit != e.kernels.end(); ++kit) {
        out << "\tID: " << kit->first << " Name: " << kit->second << "\n";
    }
    out << "\n";
    
    out << "Timings & Data volumes:\n";

    for (unsigned int i = 0; i < e.data.size(); i++) {
        
        std::list<elapsdData>::const_iterator it;
        for (it = e.data[i].begin(); it != e.data[i].end(); ++it) {
            out << *it << "\n";
        }

    }

    out << "\n";

    return out;
}
}
