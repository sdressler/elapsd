#ifndef I_PREDICTION_MODEL_H_INC_
#define I_PREDICTION_MODEL_H_INC_

#include <vector>
#include <map>
#include <string>

namespace ENHANCE {

template<class Point>
class IPredictionModel {

    friend class predict;

protected:
    typedef typename Point::first_type x_type;
    typedef typename Point::second_type y_type;

    virtual y_type getRuntimePrediction(const x_type &x) const = 0;
    virtual void updateDataPoints(const std::vector<Point> &points) = 0;

public:
    virtual ~IPredictionModel() { }

};
}

#endif 
