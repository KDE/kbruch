/***************************************************************************
                          ratio.cpp  -  source code of class ratio
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001-2004 by Sebastian Stein
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

#include <kdebug.h>

#include "ratio.h"
#include "primenumber.h"

/* ----- public member functions ----- */

/* constructor */
ratio::ratio(int pnumerator, int pdenominator):m_numerator(pnumerator), m_denominator(pdenominator)
{
#ifdef DEBUG
	kdDebug() << "constructor ratio" << endl;
#endif

	// denominator is never allowed to be 0
	if (!m_denominator)
		m_denominator = 1;

	// reduce the new ratio
	reduce();
}

/* copy constructor */
ratio::ratio(const ratio & copy_ratio)
{
#ifdef DEBUG
	kdDebug() << "copy constructor ratio" << endl;
#endif

	setNumerator(copy_ratio.numerator(), false);
	setDenominator(copy_ratio.denominator(), false);
}

/* destructor */
ratio::~ratio()
{
#ifdef DEBUG
	kdDebug() << "destructor ratio" << endl;
#endif
}

/* displays the ratio on stdout; just for debugging */
QTextStream & ratio::display(QTextStream & str) const
{
	int tmp_width = str.width();
	str << qSetW(5) << " ";
	str << qSetW(5) << m_numerator << endl;
	str << qSetW(tmp_width) << " ";
	str << " ----- " << endl;
	str << qSetW(tmp_width) << " ";
	return str << qSetW(5) << m_denominator;
}

/* return the numerator */
int ratio::numerator() const
{
	return m_numerator;
}

/* return the denominator */
int ratio::denominator() const
{
	return m_denominator;
}

/* set the numerator */
void ratio::setNumerator(int pnumerator, bool reduce_it)
{
	m_numerator = pnumerator;

	// check, if we have to reduce the ratio
	if (reduce_it == true)
		reduce();

	return;
}

/* set the denominator */
void ratio::setDenominator(int pdenominator, bool reduce_it)
{
	/* denominator is not allowed to be 0 */
	if (!pdenominator)
		pdenominator = 1;

	m_denominator = pdenominator;

	// check, if we have to reduce the ratio
	if (reduce_it == true)
		reduce();

	return;
}

/* add a ratio to a ratio like c = a + b */
ratio ratio::operator+(ratio addend)
{
	// this object will be returned as the sum
	ratio sum(0, 1);

	// calculate and set the numerator without reducing
	sum.setNumerator(m_numerator * addend.denominator()
                  + addend.numerator() * m_denominator, false);

	// calculate and set the denominator without reducing
	sum.setDenominator(m_denominator * addend.denominator(), false);

	// reduce the sum
	sum.reduce();

	return sum;
}

/* sub a ratio from a ratio like c = a - b */
ratio ratio::operator-(ratio subtrahend)
{
	/* this object will be returned as the difference */
	ratio diff(0, 1);

	/* change the sign of the subtrahend, so we can handle it as an addition */
	subtrahend.change_sign();
	diff = operator+(subtrahend);

	/* we have to change the sign back, so everything will be as before */
	subtrahend.change_sign();

	/* return the difference */
	return diff;
}

/* mul a ratio with a ratio like c = a * b */
ratio ratio::operator*(ratio factor)
{
	// this object will be returned as the product
	ratio product(0, 1);

	// calculate and set numerator and denominator without reducing
	product.setNumerator(m_numerator * factor.numerator(), false);
	product.setDenominator(m_denominator * factor.denominator(), false);

	// reduce the product
	product.reduce();

	return product;
}

/* div a ratio with a ratio like c = a / b */
ratio ratio::operator/(ratio divisor)
{
	/* this object will be returned as the quotient */
	ratio quotient(0, 1);

	/* exchange numerator and denominator so we can handle as multiplication */
	divisor.reziproc();
	quotient = operator*(divisor);

	/* go back to the original state */
	divisor.reziproc();

	return quotient;
}

/* we need this for initialization during a function prototyp;
 * ratio fraction = 0 */
ratio ratio::operator=(int dummy)
{
	m_numerator = dummy;
	m_denominator = 1;

	return *this;
}

/* check, if the ratios are equivalent; -1/2 == 1/-2 -> TRUE */
bool ratio::operator==(ratio right)
{
	signed short orig_sign = 1, right_sign = 1;

	/* we do not check the presign at this point */
	if (QABS(m_numerator) != QABS(right.numerator()))
		return false;
	if (QABS(m_denominator) != QABS(right.denominator()))
		return false;

	/* check if the signs of the ratios are equivalent */
	if (m_numerator < 0)
		orig_sign = -1;
	if (m_denominator < 0)
		orig_sign *= -1;
	if (right.numerator() < 0)
		right_sign = -1;
	if (right.denominator() < 0)
		right_sign *= -1;

	if (orig_sign != right_sign)
		return false;

	return true;
}

bool ratio::operator<(ratio right)
{
	signed short sign = 1;
	ratio tmp_ratio = ratio(m_numerator, m_denominator) - right;

	// check for this == right
	if (tmp_ratio == ratio(0, 1))
		return false;

	// get the presign of the diff
	if (tmp_ratio.numerator() < 0)
		sign = -1;
	if (tmp_ratio.denominator() < 0)
		sign *= -1;

	// if the diff is negative, this is smaller then right
	if (sign > 0)
	{
		return false;
	} else {
		return true;
	}
}

bool ratio::operator>(ratio right)
{
	signed short sign = 1;
	ratio tmp_ratio = ratio(m_numerator, m_denominator) - right;

	// check for this == right
	if (tmp_ratio == ratio(0, 1))
		return false;

	// get the presign of the diff
	if (tmp_ratio.numerator() < 0)
		sign = -1;
	if (tmp_ratio.denominator() < 0)
		sign *= -1;

	// if the diff is positive, this is smaller then right
	if (sign < 0)
	{
		return false;
	} else {
		return true;
	}
}

/* ----- private member functions ----- */

/* reduce the ratio */
void ratio::reduce()
{
	/* we try prime numbers as divisors; I think it is the fastet way to do */
	primenumber number;
	short sign_numerator = 0, sign_denominator = 0;

	/* make the whole ratio positive; save the signs; it is easier to reduce
	 * the ratio, if it is positive */
	if (m_numerator < 0) // save numerator sign
	{
		sign_numerator = 1;
		m_numerator *= -1;
	}
	if (m_denominator < 0) // save denominator sign
	{
		sign_denominator = 1;
		m_denominator *= -1;
	}

	for (int divisor = number.get_first();
	        divisor <= m_numerator && divisor <= m_denominator; divisor = number.get_next())
	{
		if (divisor == 0)
		{
#ifdef DEBUG
			kdDebug() << "ratio::reduce() -> divisor == 0 !!!" << endl;
			kdDebug() << "m_numerator: " << m_numerator << endl;
			kdDebug() << "m_denominator: " << m_denominator << endl;
			// cin.get();
#endif
			/* so that the application does not crash with a floating
			 * point exception; the error should not appear, but in some
			 * cases it does and I do not know why */
			continue;
		}

		/* is the prime number a divisor of numerator and denominator? */
		if ((m_numerator % divisor == 0) && (m_denominator % divisor == 0))
		{
			/* reduce the ratio by the divisor */
			m_numerator /= divisor;
			m_denominator /= divisor;

			/* we have to go recursive, if the 2 is a divisor, because there
			 * is no way to step one number before 2 -> there is no prime
			 * number smaller than 2 */
			if (divisor == 2)
				reduce();
			else
				number.move_back(); // the prime number could be a divisor again
		} // if ((zaehler % divisor == 0) && (nenner % divisor == 0))
	} // for (unsigned int divisor = number.get_first(); ...

	/* restore the correct signs */
	if (sign_numerator)
		m_numerator *= -1;
	if (sign_denominator)
		m_denominator *= -1;
	if (m_numerator == 0)
		m_denominator = 1;

	return;
}

/* exchange numerator and denominator */
void ratio::reziproc()
{
	int temp = m_numerator;
	m_numerator = m_denominator;
	m_denominator = temp;

	return;
}


/* ------ private member functions ------ */

/* change the sign of the ratio; ratio = ratio * -1 */
void ratio::change_sign()
{
	/* this would be enough to change the sign of the ratio */
	m_numerator *= -1;

	/* if numerator and denominator both are negative, make them positive;
	 * if denominator is negative and numerator positive, exchange the sign */
	if ((m_numerator < 0 && m_denominator < 0) || (m_numerator > 0 && m_denominator < 0))
	{
		m_numerator *= -1;
		m_denominator *= -1;
	}

	return;
}


/* ------ some prototyps of non class functions ------ */

// it is possible to stram ratio_object
QTextStream & operator<<(QTextStream & str, const ratio & pratio)
{
	return pratio.display(str);
}
