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
#include "primenumber.h"


class primenumber_test
{
	public:

	// constructor
	primenumber_test()
	{
	}

	/** test the prime number algorithm */
	void test_isPrimeNumber()
	{
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(0) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(2) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(3) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(4) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(5) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(6) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(7) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(8) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(9) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(23) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(9) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(9) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(6) == 0);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(101) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(323) == 0); // 17 * 19
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(1001) == 0); // 7 * 143
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(1002) == 0); // 2 * 501
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(3) == 1);
		BOOST_REQUIRE(m_primenumber.isPrimeNumber(2) == 1);
	}

	/** test the get_first() function */
	void test_get_first()
	{
		BOOST_REQUIRE(m_primenumber.get_first() == 2);
	}

	/** test the move and get functions */
	void test_move_get_func()
	{
		m_primenumber.move_first();
		BOOST_REQUIRE(m_primenumber.get_current() == 2);
		BOOST_REQUIRE(m_primenumber.get_next() == 3);

		m_primenumber.move_forward();
		BOOST_REQUIRE(m_primenumber.get_current() == 5);

		m_primenumber.move_back();
		BOOST_REQUIRE(m_primenumber.get_current() == 3);
		
		unsigned int tmp = m_primenumber.get_last();
		m_primenumber.move_last();
		BOOST_REQUIRE(m_primenumber.get_current() == tmp);

		m_primenumber.move_forward();
		BOOST_REQUIRE(m_primenumber.get_last() != tmp);
	}

	private:

	// instance of primenumber
	primenumber m_primenumber;
};

class primenumber_test_suite : public test_suite
{
	public:

	primenumber_test_suite() : test_suite("primenumber_test_suite")
	{
		// create an instance of the test cases class
		boost::shared_ptr<primenumber_test> instance(new primenumber_test());

		// create the test cases
		test_case* isPrimeNumber_test_case = BOOST_CLASS_TEST_CASE(
										&primenumber_test::test_isPrimeNumber, instance );
		test_case* get_first_test_case = BOOST_CLASS_TEST_CASE(
										&primenumber_test::test_get_first, instance );
		test_case* move_get_func_test_case = BOOST_CLASS_TEST_CASE(
										&primenumber_test::test_move_get_func, instance );

		// add the test cases to the test suite
		add(isPrimeNumber_test_case);
		add(get_first_test_case);
		add(move_get_func_test_case);
	}
};
