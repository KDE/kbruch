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
using boost::unit_test_framework::test_case;

// the class to be tested
#include "../src/primzahl.h"


class primzahl_test
{
	public:

	// constructor
	primzahl_test()
	{
	}

	void test_isPrimeNumber()
	{
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(2) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(3) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(4) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(5) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(6) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(7) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(8) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(9) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(23) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(9) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(9) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(6) == 0);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(101) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(323) == 0); // 17 * 19
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(1001) == 0); // 7 * 143
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(1002) == 0); // 2 * 501
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(3) == 1);
		BOOST_REQUIRE(m_primzahl.isPrimeNumber(2) == 1);
	}

	private:

	// instance of primzahl
	primzahl m_primzahl;
};

class primzahl_test_suite : public test_suite
{
	public:

	primzahl_test_suite() : test_suite("primzahl_test_suite")
	{
		// create an instance of the test cases class
		boost::shared_ptr<primzahl_test> instance(new primzahl_test());

		// create the test cases
		test_case* isPrimeNumber_test_case = BOOST_CLASS_TEST_CASE(
										&primzahl_test::test_isPrimeNumber, instance );

		// add the test cases to the test suite
		add(isPrimeNumber_test_case);
	}
};
