#ifndef LAGRANGE_H_INC_
#define LAGRANGE_H_INC_

#include <elapsd/Helpers.h>

#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>
#include <stdexcept>
#include <sstream>

namespace ENHANCE {

/**
 * This class implements the calculation of a Lagrange polynomial.
 * */

template<int N, class X, class Y>
class Lagrange {

public:
    typedef std::pair<X, Y> Point;

private:
    std::vector<Point> points_;
    std::vector<Point> current_best_points_;

    double current_error_;

public:
    Lagrange() {
        current_error_ = std::numeric_limits<double>::infinity();
    }

    Lagrange(const std::vector<Point> &points) { updatePoints(points); }

    Lagrange(const Lagrange &L) {
        points_ = L.points_;
        current_best_points_ = L.current_best_points_;
        current_error_ = L.current_error_;
    }

    void updatePoints(const std::vector<Point> &points) {
        
        if (points.size() < N) {
            throw std::runtime_error("Not enough points.");
        }

        points_ = points;

        /* To be sure... */
        current_best_points_.clear();
        current_error_ = std::numeric_limits<double>::infinity();

    }

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

                std::vector<Point> tmp_points;     // Used as "best points"
                std::vector<Point> tmp_cmp_points; // Used for comparison

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

                typename std::vector<Point>::const_iterator it =
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

            DMSG("Min Error: " << current_error_);

        }

    }

    /**
     * Compute a single Lagrange polynomial, as basis take the
     * current_best_points_. If they are not yet deployed, throw an error. This
     * returns the interpolated value of type Y and takes the requested value of
     * type X.
     * */
    Y getLagrangeInterpolation(const X &x) const {
    
        if (current_best_points_.empty()) {
            throw std::runtime_error(
                "Best points not deployed. Run 'findBestPolynomial' first."
            );
        }

        // Note: internally, we compute everything with double precision.
        double y_int = 0.0, x_int = static_cast<double>(x);

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

        return static_cast<Y>(y_int);

    }

};

}

#endif /* LAGRANGE_H_INC_ */
