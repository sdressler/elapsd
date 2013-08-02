#define BOOST_TEST_STATIC_LINK
#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;

void test_case_foo() {
    BOOST_CHECK( true /* test assertion */ );
}

test_suite* init_unit_test_suite( int argc, char* argv[] ) {

    test_suite* ts = BOOST_TEST_SUITE("test_suite");
/*
    for(int i=0; i < 10000; i++) {
        ts->add(BOOST_TEST_CASE(&test_case_foo));
    }
*/
    framework::master_test_suite().add(ts);

    return 0;
}

