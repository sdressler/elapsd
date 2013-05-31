#ifndef ELAPSD_EXPERIMENT_H
#define ELAPSD_EXPERIMENT_H

#include <string>
#include <vector>
#include <sstream>

#include <ctime>

namespace ENHANCE {
class elapsdExperiment {
private:
	std::string name;       ///< Experiment identifier
    std::string osinfo;     ///< Version of OS
    std::string machine;    ///< Machine info
    
    time_t start;       ///< Start of experiment
    time_t stop;        ///< Stop of experiment

public:
	/**
	 *
	 * Default constructor.
	 *
	 * */
    elapsdExperiment(const std::string &_name);

	/**
	 *
	 * Print the device name to a stream. If the stream is a file the
	 * output is converted to JSON.
	 *
	 * @param[in,out] out The corresponding stream
	 * @param[in] d A reference to the data
	 * @retval std::ostream The returning stream
	 * */
	friend std::ostream& operator<<(std::ostream &out, const elapsdExperiment &e);

//	virtual tByteVectorMap convertToByteVectorMap() const;
};
}

#endif /* ELAPSD_EXPERIMENT_H */
