#include "../include/elapsd/Kernel.h"

#include <cstring>

namespace ENHANCE {
elapsdKernel::elapsdKernel(int _id, std::string s) {
    id = _id;
    name = s;
}

std::vector<std::string> elapsdKernel::createSQLInsertObj() const {

    std::vector<std::string> x;

    std::stringstream q;
    
    q   << "INSERT OR IGNORE INTO kernels (id, name) VALUES("
        << id << ", '" << name << "')";

    x.push_back(q.str());

    return x;

}

std::ostream& operator<<(std::ostream &out, const elapsdKernel &k) {
    out << k.name;
    return out;
}

}
