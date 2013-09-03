#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elapsd/predict/predict.h>
#include <elapsd/Params.h>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(constructors_test) {

    std::vector<std::string> params;

    BOOST_CHECK_THROW(
        ENHANCE::predict p("", params),
        std::runtime_error
    );

    BOOST_CHECK_THROW(
        ENHANCE::predict p("this_db_does_not_exist.db", params),
        std::runtime_error
    );

    BOOST_CHECK_THROW(
        ENHANCE::predict p("test.db", params),
        std::runtime_error
    );

    /* Populate list with *wrong* parameters */
    params.push_back("FAIL");
    BOOST_CHECK_THROW(
        ENHANCE::predict p("test.db", params),
        std::runtime_error
    );

    /* Re-populate with correct parameters */
    params.clear();
    params.push_back("FIB");
    BOOST_CHECK_NO_THROW(ENHANCE::predict p("test.db", params));

}

BOOST_AUTO_TEST_CASE(distinct_measurements_test) {

    std::vector<std::string> params;
    params.push_back("FIB");

    ENHANCE::predict *p = NULL;

    BOOST_REQUIRE_NO_THROW(
        p = new ENHANCE::predict("test.db", params)
    );

    BOOST_CHECK_EQUAL(p->getNumberOfDistinctMeasurements(), 4);
    
    delete p;

    BOOST_REQUIRE_NO_THROW(
        p = new ENHANCE::predict("test.db", params, 1, 1)
    );

    BOOST_CHECK_EQUAL(p->getNumberOfDistinctMeasurements(), 0);

    delete p;

}

BOOST_AUTO_TEST_CASE(prediction_test) {

    std::vector<std::string> params;
    params.push_back("FIB");

    ENHANCE::predict *p = NULL;

    BOOST_REQUIRE_NO_THROW(
        p = new ENHANCE::predict("test.db", params);
    );

    BOOST_REQUIRE_NO_THROW(p->generateLagrangePolynomial());

    double runtime = 0.0;

    BOOST_REQUIRE_NO_THROW(runtime = p->makeRuntimePrediction(50));

    BOOST_CHECK(runtime < 0.56);
    BOOST_CHECK(runtime > 0.54);

    delete p;

}
