/***************************************************************************

    begin                : 2004-06-03
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
#include "ratio.h"


class ratio_test
{
	public:

	// constructor
	ratio_test()
	{
	}

	/** test the constructor of class ratio */
	void test_constructor()
	{
		// check basic initialisation
		ratio * ratio_a = new ratio();
		BOOST_REQUIRE(ratio_a->numerator() == 0);
		BOOST_REQUIRE(ratio_a->denominator() == 1);

		// a denominator with value 0 is never allowed
		ratio * ratio_b = new ratio(2, 0);
		BOOST_REQUIRE(ratio_b->numerator() == 2);
		BOOST_REQUIRE(ratio_b->denominator() == 1); // division by zero not allowed

		// a new ratio should always be reduced
		ratio * ratio_c = new ratio(2, 4);
		BOOST_REQUIRE(ratio_c->numerator() == 1);
		BOOST_REQUIRE(ratio_c->denominator() == 2);

		// check copy constructor
		ratio * ratio_d = new ratio(*ratio_c);
		BOOST_REQUIRE(ratio_d->numerator() == 1);
		BOOST_REQUIRE(ratio_d->denominator() == 2);

		// the copy constructor is not allowed to reduce the new ratio
		ratio_d->setNumerator(4, false);
		ratio * ratio_e = new ratio(*ratio_d);
		BOOST_REQUIRE(ratio_e->numerator() == 4);
		BOOST_REQUIRE(ratio_e->denominator() == 2);

		delete(ratio_a);
		delete(ratio_b);
		delete(ratio_c);
		delete(ratio_d);
		delete(ratio_e);

		return;
	}

	/** test set and get functions */
	void test_setAndGet()
	{
		// check setNumerator() and numerator() (get)
		ratio * ratio_a = new ratio();
		ratio_a->setNumerator(10);
		BOOST_REQUIRE(ratio_a->numerator() == 10);
		BOOST_REQUIRE(ratio_a->denominator() == 1);
		
		// check setDenominator() and denominator() (get)
		ratio_a->setDenominator(7);
		BOOST_REQUIRE(ratio_a->numerator() == 10);
		BOOST_REQUIRE(ratio_a->denominator() == 7);

		// now check if the ratio gets reduced
		ratio_a->setNumerator(14);
		BOOST_REQUIRE(ratio_a->numerator() == 2);
		BOOST_REQUIRE(ratio_a->denominator() == 1);

		// lets do the same, but with out reducing
		ratio_a->setNumerator(14, false);
		ratio_a->setDenominator(7, false);
		BOOST_REQUIRE(ratio_a->numerator() == 14);
		BOOST_REQUIRE(ratio_a->denominator() == 7);

		// now the = operator
		ratio * ratio_b = new ratio();
		*ratio_b = *ratio_a;
		BOOST_REQUIRE(ratio_b->numerator() == 14);
		BOOST_REQUIRE(ratio_b->denominator() == 7);

		// make sure we didn't just copied the pointers
		BOOST_REQUIRE(ratio_a != ratio_b);

		delete(ratio_a);
		delete(ratio_b);

		return;
	}

	/** test the = operator */
	void test_operatorSet()
	{
		ratio ratio_a;
		ratio_a = 8;
		BOOST_REQUIRE(ratio_a.numerator() == 8);
		BOOST_REQUIRE(ratio_a.denominator() == 1);

		ratio ratio_b(2, 3);
		ratio_a =  ratio_b;
		BOOST_REQUIRE(ratio_a.numerator() == 2);
		BOOST_REQUIRE(ratio_a.denominator() == 3);

		return;
	}

	/** test the + and - operator */
	void test_operatorAddSub()
	{
		ratio * ratio_a = new ratio();
		ratio_a->setNumerator(4, false);
		ratio_a->setDenominator(2, false);

		ratio * ratio_b = new ratio();
		ratio_b->setNumerator(4, false);
		ratio_b->setDenominator(8, false);

		// check + operator
		ratio ratio_c = *ratio_a + *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 5);
		BOOST_REQUIRE(ratio_c.denominator() == 2);

		// it should be the same if we change the addends
		ratio_c = *ratio_b + *ratio_a;
		BOOST_REQUIRE(ratio_c.numerator() == 5);
		BOOST_REQUIRE(ratio_c.denominator() == 2);

		// check - operator
		ratio_c = *ratio_b - *ratio_a;
		BOOST_REQUIRE(ratio_c.numerator() == -3);
		BOOST_REQUIRE(ratio_c.denominator() == 2);

		// it should not be the same if we change the subtrahends
		ratio_c = *ratio_a - *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 3);
		BOOST_REQUIRE(ratio_c.denominator() == 2);

		// now check if we can get 0/1
		*ratio_a = *ratio_b;
		ratio_c = *ratio_a - *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 0);
		BOOST_REQUIRE(ratio_c.denominator() == 1);
		
		delete(ratio_a);
		delete(ratio_b);

		return;
	}

	/** test the * and / operator */
	void test_operatorMulDiv()
	{
		ratio * ratio_a = new ratio();
		ratio_a->setNumerator(4, false);
		ratio_a->setDenominator(2, false);

		ratio * ratio_b = new ratio();
		ratio_b->setNumerator(4, false);
		ratio_b->setDenominator(16, false);

		// check * operator
		ratio ratio_c = *ratio_a * *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 1);
		BOOST_REQUIRE(ratio_c.denominator() == 2);

		// it should be the same if we change the addends
		ratio_c = *ratio_b * *ratio_a;
		BOOST_REQUIRE(ratio_c.numerator() == 1);
		BOOST_REQUIRE(ratio_c.denominator() == 2);

		// check / operator
		ratio_c = *ratio_b / *ratio_a;
		BOOST_REQUIRE(ratio_c.numerator() == 1);
		BOOST_REQUIRE(ratio_c.denominator() == 8);

		// it should not be the same if we change the subtrahends
		ratio_c = *ratio_a / *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 8);
		BOOST_REQUIRE(ratio_c.denominator() == 1);

		// now check if we can get 0/1
		*ratio_a = 0;
		ratio_c = *ratio_a * *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 0);
		BOOST_REQUIRE(ratio_c.denominator() == 1);
		ratio_c = *ratio_a / *ratio_b;
		BOOST_REQUIRE(ratio_c.numerator() == 0);
		BOOST_REQUIRE(ratio_c.denominator() == 1);
		
		delete(ratio_a);
		delete(ratio_b);

		return;
	}

	/** test function reziproc() */
	void test_reziproc()
	{
		ratio ratio_a(2, 3);
		ratio_a.reziproc();
		BOOST_REQUIRE(ratio_a.numerator() == 3);
		BOOST_REQUIRE(ratio_a.denominator() == 2);

		return;
	}

	/** test function reduce() */
	void test_reduce()
	{
		ratio ratio_a;
		ratio_a.setNumerator(51, false);
		ratio_a.setDenominator(17, false);
		ratio_a.reduce();
		BOOST_REQUIRE(ratio_a.numerator() == 3);
		BOOST_REQUIRE(ratio_a.denominator() == 1);

		return;
	}

	/** test operator compare */
	void test_operatorCompare()
	{
		ratio ratio_a(2, 3);
		ratio ratio_b(2, 3);
		ratio ratio_c(-2, -3);
		ratio ratio_d(2, -3);
		ratio ratio_e(-2, 3);

		BOOST_REQUIRE((ratio_a == ratio_a) == true);
		BOOST_REQUIRE((ratio_a == ratio_b) == true);
		BOOST_REQUIRE((ratio_a == ratio_c) == true);
		BOOST_REQUIRE((ratio_a == ratio_d) == false);
		BOOST_REQUIRE((ratio_a == ratio_e) == false);

		return;
	}

	/** test operator smaller */
	void test_operatorSmaller()
	{
		ratio ratio_a(2, 3);
		ratio ratio_b(2, 3);
		ratio ratio_c(-2, -3);
		ratio ratio_d(2, -3);
		ratio ratio_e(-2, 3);

		BOOST_REQUIRE((ratio_a < ratio_a) == false);
		BOOST_REQUIRE((ratio_a < ratio_b) == false);
		BOOST_REQUIRE((ratio_a < ratio_c) == false);
		BOOST_REQUIRE((ratio_a < ratio_d) == false);
		BOOST_REQUIRE((ratio_a < ratio_e) == false);

		ratio_a = ratio(-2, 3);

		BOOST_REQUIRE((ratio_a < ratio_a) == false);
		BOOST_REQUIRE((ratio_a < ratio_b) == true);
		BOOST_REQUIRE((ratio_a < ratio_c) == true);
		BOOST_REQUIRE((ratio_a < ratio_d) == false);
		BOOST_REQUIRE((ratio_a < ratio_e) == false);

		BOOST_REQUIRE((ratio(5, 8) < ratio(2, 1)) == true);

		return;
	}

	/** test operator bigger */
	void test_operatorBigger()
	{
		ratio ratio_a(2, 3);
		ratio ratio_b(2, 3);
		ratio ratio_c(-2, -3);
		ratio ratio_d(2, -3);
		ratio ratio_e(-2, 3);
		ratio ratio_f(-4, 3);

		BOOST_REQUIRE((ratio_a > ratio_a) == false);
		BOOST_REQUIRE((ratio_a > ratio_b) == false);
		BOOST_REQUIRE((ratio_a > ratio_c) == false);
		BOOST_REQUIRE((ratio_a > ratio_d) == true);
		BOOST_REQUIRE((ratio_a > ratio_e) == true);
		BOOST_REQUIRE((ratio_a > ratio_f) == true);

		ratio_a = ratio(-2, 3);

		BOOST_REQUIRE((ratio_a > ratio_a) == false);
		BOOST_REQUIRE((ratio_a > ratio_b) == false);
		BOOST_REQUIRE((ratio_a > ratio_c) == false);
		BOOST_REQUIRE((ratio_a > ratio_d) == false);
		BOOST_REQUIRE((ratio_a > ratio_e) == false);
		BOOST_REQUIRE((ratio_a > ratio_f) == true);

		return;
	}

	private:

	// instance of primenumber
	ratio m_ratio;
};

class ratio_test_suite : public test_suite
{
	public:

	ratio_test_suite() : test_suite("ratio_test_suite")
	{
		// create an instance of the test cases class
		boost::shared_ptr<ratio_test> instance(new ratio_test());

		// create the test cases
		test_case* constructor_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_constructor, instance );
		test_case* setAndGet_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_setAndGet, instance );
		test_case* operatorSet_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_operatorSet, instance );
		test_case* operatorAddSub_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_operatorAddSub, instance );
		test_case* operatorMulDiv_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_operatorMulDiv, instance );
		test_case* reziproc_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_reziproc, instance );
		test_case* reduce_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_reduce, instance );
		test_case* operatorCompare_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_operatorCompare, instance );
		test_case* operatorSmaller_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_operatorSmaller, instance );
		test_case* operatorBigger_test_case = BOOST_CLASS_TEST_CASE(
										&ratio_test::test_operatorBigger, instance );

		// add the test cases to the test suite
		add(constructor_test_case);
		add(setAndGet_test_case);
		add(operatorSet_test_case);
		add(operatorAddSub_test_case);
		add(operatorMulDiv_test_case);
		add(reziproc_test_case);
		add(reduce_test_case);
		add(operatorCompare_test_case);
		add(operatorSmaller_test_case);
		add(operatorBigger_test_case);
	}
};
