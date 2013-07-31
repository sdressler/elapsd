#define BOOST_TEST_MODULE elapsd_predict test
#include <boost/test/unit_test.hpp>

#include <elapsd/predict/predict.h>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(constructors_test) {

    BOOST_CHECK_THROW(ENHANCE::predict p(""), std::runtime_error);

    BOOST_CHECK_THROW(
        ENHANCE::predict p("this_db_does_not_exist.db"),
        std::runtime_error
    );

    BOOST_CHECK_NO_THROW(ENHANCE::predict p("test.db"))

}
