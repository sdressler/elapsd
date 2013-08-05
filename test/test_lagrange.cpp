#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <elapsd/predict/Lagrange.h>

using namespace boost::unit_test;

/**
 * We need this class, since BOOST_CHECK_THROW does not work with templates
 * */
class Lagrange_3_int_double : public ENHANCE::Lagrange<3, int, double> {

public:
    Lagrange_3_int_double(const std::vector<std::pair<int, double> > &points)
        : ENHANCE::Lagrange<3, int, double>(points) { }

};

/* Test constructors of Lagrange class */
BOOST_AUTO_TEST_CASE(lagrange_constructor) {

    std::vector<std::pair<int, double> > points;

    points.push_back(std::make_pair(1, 1.0));
    points.push_back(std::make_pair(2, 2.0));

    BOOST_CHECK_THROW(Lagrange_3_int_double L(points), std::runtime_error);
    
    points.push_back(std::make_pair(3, 3.0));

    BOOST_CHECK_NO_THROW(Lagrange_3_int_double L(points));

}

BOOST_AUTO_TEST_CASE(lagrange_interpolation_simple) {

    /**
     * Test, that the polynomial 0.5x^2 - 0.5x + 1 is generated
     * */

    std::vector<std::pair<int, double> > points;

    points.push_back(std::make_pair(1, 1.0));
    points.push_back(std::make_pair(2, 2.0));
    points.push_back(std::make_pair(3, 4.0));

    ENHANCE::Lagrange<3, int, double> L(points);

    int x = 4;
    double y = 0.0;

    BOOST_CHECK_THROW(y = L.getLagrangeInterpolation(x), std::runtime_error);

    // Call the needed function
    BOOST_REQUIRE_NO_THROW(L.findBestPolynomial());

    BOOST_REQUIRE_NO_THROW(y = L.getLagrangeInterpolation(x));

    BOOST_CHECK(y < 7.01);
    BOOST_CHECK(y > 6.99);

    x = 0;
    BOOST_REQUIRE_NO_THROW(y = L.getLagrangeInterpolation(x));

    BOOST_CHECK(y < 1.01);
    BOOST_CHECK(y > 0.99);

}

BOOST_AUTO_TEST_CASE(lagrange_interpolation_complex) {

    /**
     * This tests, whether multiple lagrange polynomials are generated and the
     * best of them gets selected.
     * */

    typedef ENHANCE::Lagrange<3, int, double>::Point Point;

    std::vector<Point> points;

    points.push_back(std::make_pair(1,  0.95));
    points.push_back(std::make_pair(2,  2.03));
    points.push_back(std::make_pair(3,  3.96));
    points.push_back(std::make_pair(4,  8.10));
    points.push_back(std::make_pair(5, 15.89));

    ENHANCE::Lagrange<3, int, double> L(points);

    BOOST_REQUIRE_NO_THROW(L.findBestPolynomial());

    /**
     * Should select the polynomial around x = {1,3,5}, thus we compare with a
     * polynomial generated from these points.
     * */
    std::vector<Point> cmp_points;

    cmp_points.push_back(points[0]);
    cmp_points.push_back(points[2]);
    cmp_points.push_back(points[4]);

    ENHANCE::Lagrange<3, int, double> M(cmp_points);

    BOOST_REQUIRE_NO_THROW(M.findBestPolynomial());

    // Equal check over a range
    for (int i = 1; i < 100; i++) {
        BOOST_CHECK_EQUAL(
            L.getLagrangeInterpolation(i),
            M.getLagrangeInterpolation(i)        
        );
    }

    points.push_back(std::make_pair(6, 32.13));

    BOOST_REQUIRE_NO_THROW(L.updatePoints(points));
    BOOST_CHECK_THROW(L.getLagrangeInterpolation(0), std::runtime_error);
    BOOST_REQUIRE_NO_THROW(L.findBestPolynomial());

    cmp_points[2] = points[5];
    
    ENHANCE::Lagrange<3, int, double> N(cmp_points);
    
    BOOST_REQUIRE_NO_THROW(N.findBestPolynomial());

    // Equal check over a range
    for (int i = 1; i < 100; i++) {
        BOOST_CHECK_EQUAL(
            L.getLagrangeInterpolation(i),
            N.getLagrangeInterpolation(i)        
        );
    }

}
