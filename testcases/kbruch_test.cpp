/***************************************************************************

    begin                : 2004-05-25
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// for BOOST testing
#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

// the test classes
#include "primenumber_test.cpp"
#include "ratio_test.cpp"

// test program entry point
test_suite* init_unit_test_suite(int /* argc */, char** /* argv */)
{
	// create the top test suite
	std::auto_ptr<test_suite> top_test_suite(BOOST_TEST_SUITE("Master test suite"));

	// add test suites to the top test suite
	top_test_suite->add(new primenumber_test_suite());
	top_test_suite->add(new ratio_test_suite());

	return top_test_suite.release();
}
