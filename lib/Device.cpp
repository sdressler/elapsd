#include "../include/elapsd/Device.h"

#include <cstring>

namespace ENHANCE {

elapsdDevice::elapsdDevice(int _id, std::string n) {
    id = _id;
	name = n;
}
	
void elapsdDevice::addSubDevice(int s) {
	subDevices.push_back(s);
}

std::ostream& operator<<(std::ostream &out, const elapsdDevice &d) {
	out << d.name;

	if (d.subDevices.size() != 0) {
		out << " Subdevices: ";
		std::vector<int>::const_iterator it;
		for (it = d.subDevices.begin(); it != d.subDevices.end(); ++it) {
			out << *it << " ";
		}
	}

	return out;
}

std::vector<std::string> elapsdDevice::createSQLInsertObj() const {

    std::vector<std::string> x;

    std::stringstream q;
    
    q   << "INSERT OR IGNORE INTO devices (id, name) VALUES("
        << id << ", '" << name << "')";

    x.push_back(q.str());

    std::vector<int>::const_iterator it;
    for (it = subDevices.begin(); it != subDevices.end(); ++it) {

        q.str("");
        q.clear();

        q << "INSERT OR IGNORE INTO subdevices (id_left, id_right) VALUES("
          << id << ", " << *it << ")";

        x.push_back(q.str());

    }

    return x;

}
}
