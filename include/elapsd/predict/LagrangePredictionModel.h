#ifndef LAGRANGE_PREDICTION_MODEL_H_INC_
#define LAGRANGE_PREDICTION_MODEL_H_INC_

#include "IPredictionModel.h"

#include <stdexcept>

#include <limits>
#include <cmath>
#include <algorithm>

#include <elapsd/Helpers.h>

namespace ENHANCE {

class LagrangePredictionModel
    : public IPredictionModel<std::pair<std::vector<int>, float> > {

private:
    typedef std::pair<std::vector<int>, float> MultDimPointT;
    typedef std::pair<int, float> PointT;

    typedef IPredictionModel<MultDimPointT>::x_type x_type;
    typedef IPredictionModel<MultDimPointT>::y_type y_type;
    
    std::vector<PointT> points_;
    std::vector<PointT> current_best_points_;

    double current_error_;

    static const int N = 3;

public:

    virtual y_type getRuntimePrediction(const x_type &x) const {
        return getLagrangeInterpolation(x.front());
    }

    virtual void updateDataPoints(const std::vector<MultDimPointT> &points) {

        // Currently only one free parameter is allowed
        if (!points.empty() && points.front().first.size() > 1) {
            throw std::runtime_error(
                "This model currently allows only one free parameter."
            );
        }

        if (points.empty()) {
            throw std::invalid_argument("No data points available.");
        }

        std::map<
            PointT::first_type, std::pair<PointT::second_type, int>
        > tmp_points_map;

        std::vector<MultDimPointT>::const_iterator it = points.begin();
        std::map<
            PointT::first_type, std::pair<PointT::second_type, int>
        >::iterator iit;

        for (; it != points.end(); ++it) {

            PointT p(std::make_pair(it->first.front(), it->second));

            iit = tmp_points_map.find(p.first);
            if (iit == tmp_points_map.end()) {
                tmp_points_map.insert(std::make_pair(
                    p.first, std::make_pair(p.second, 1)
                ));
            } else {
                iit->second.first += p.second;
                iit->second.second++;
            }

        }
        
        if (tmp_points_map.size() < N) {
            throw std::invalid_argument("Not enough distinct points (3) provided.");
        }

        points_.clear();
        for (iit = tmp_points_map.begin(); iit != tmp_points_map.end(); ++iit) {

            points_.push_back(std::make_pair(
                iit->first,
                iit->second.first / static_cast<PointT::second_type>(
                    iit->second.second
                )
            ));

            DMSG("X: " << points_.back().first << " Y: " << points_.back().second);

        }

        findBestPolynomial();

    }

//    virtual ~LagrangePredictionModel() { }

private:
    /**
     * This routine will find the best matching polynomial for the points
     * provided in "points_". If size(points_) > N, then for every combination M
     * over N, where M = size(points_), a polynomial is generated. The best
     * matching points for a polynomial are then stored in best_points_.
     * */
    void findBestPolynomial() {

        if (points_.size() == N) {

            current_best_points_ = points_;
            current_error_ = 0.0;

        } else {

            /**
             * This requires us to generate all possible combinations where N
             * out of M points are selected. Then, interpolation is done with
             * the remaining points and the errors are summed. Subsequently, the
             * points with min(error) are selected as best points.
             * */

            int M = points_.size();

            std::vector<bool> v(M);
            std::fill(v.begin() + N, v.end(), true);

            current_error_ = std::numeric_limits<double>::infinity();

            do {

                std::vector<PointT> tmp_points;     // Used as "best points"
                std::vector<PointT> tmp_cmp_points; // Used for comparison

                for (int i = 0; i < M; ++i) {
                    if (!v[i]) {
                        tmp_points.push_back(points_[i]);
                    } else {
                        tmp_cmp_points.push_back(points_[i]);
                    }
                }

                current_best_points_.swap(tmp_points);

                /* Now interpolate "cmp" values and sum errors. */
                double error = 0.0;

                std::vector<PointT>::const_iterator it =
                    tmp_cmp_points.begin();

                for (; it != tmp_cmp_points.end(); ++it) {
                    error += std::fabs(
                        getLagrangeInterpolation(it->first) - it->second
                    );
                }

                if (error < current_error_) {
                    current_error_ = error;
                } else {
                    // If not smaller, back-swap vectors
                    current_best_points_.swap(tmp_points);    
                }

            } while (std::next_permutation(v.begin(), v.end()));

        }

    }
    
    /**
     * Compute a single Lagrange polynomial, as basis take the
     * current_best_points_. If they are not yet deployed, throw an error. This
     * returns the interpolated value of type Y and takes the requested value of
     * type X.
     * */
    y_type getLagrangeInterpolation(const PointT::first_type &x) const {
    
        DMSG("Prediction for: " << x);

        if (current_best_points_.empty()) {
            return std::numeric_limits<y_type>::infinity();
        }

        // Note: internally, we compute everything with double precision.
        double y_int = 0.0;
        double x_int = static_cast<double>(x);

        // Spawn loops for lagrange polynomial
        int k = current_best_points_.size();
    
        for (int j = 0; j < k; j++) {
    
            double l_j = 1.0;

            for (int m = 0; m < k; m++) {
                
                if (m != j) {
                    
                    double
                        x_m = static_cast<double>(current_best_points_[m].first),
                        x_j = static_cast<double>(current_best_points_[j].first);

                    l_j *= (x_int - x_m) / (x_j - x_m);

                }
            }

            y_int += l_j * static_cast<double>(current_best_points_[j].second);

        }

        return static_cast<y_type>(y_int);

    }

};
}

#endif
